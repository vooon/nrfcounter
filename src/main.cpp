/**
 * LED Blink counter.
 *
 * Each N-th blink means consumption of one kW*h.
 *
 * @author Vladimir Ermakov <vooon341@gmail.com>
 * @copyright 2019
 */

#include <array>

#include "ble/BLE.h"
#include "ble/services/EnvironmentalService.h"
#include "mbed.h"
#include "mbed_events.h"

#include "BME280.h"
#include "RGBLed.h"

#include "PowerMeterService.h"

// Leads are inverted, active LOW
RGBLed led(P0_27, P0_28, P0_29);

// I2C's
I2C i2c_ap_bme(P0_19, P0_21);
I2C i2c_bmi_bmp(P0_11, P0_12);

BME280 bme(i2c_ap_bme);

const char DEVICE_NAME[] = "Power-Meter";

EventQueue event_queue(16 * EVENTS_EVENT_SIZE);

void next_color()
{
	const std::array<RGBLed::Color, 8> colors { { RGBLed::BLACK,
	    RGBLed::RED,
	    RGBLed::GREEN,
	    RGBLed::BLUE,
	    RGBLed::MAGENTA,
	    RGBLed::CYAN,
	    RGBLed::YELLOW,
	    RGBLed::WHITE } };

	static auto it = colors.cbegin();

	auto color = *it;
	led.setColor(color);

	if (++it == colors.cend())
		it = colors.cbegin();
}

void read_sensor()
{
}

class PowerMeter : ble::Gap::EventHandler {
    public:
	PowerMeter(BLE& _ble, EventQueue& _evq)
	    : ble(_ble)
	    , evq(_evq)
	    , _pm_service(nullptr)
	    , _adv_data_builder(_adv_buffer)
	{
	}

	void start()
	{
		ble.gap().setEventHandler(this);
		ble.init(this, &PowerMeter::on_init_complete);

		//evq.call_every(500, this, &LEDDemo::blink);

		evq.dispatch_forever();
	}

    private:
	/// Callback triggered when the ble initialization process has finished
	void on_init_complete(BLE::InitializationCompleteCallbackContext* params)
	{
		if (params->error != BLE_ERROR_NONE) {
			//printf("Ble initialization failed.");
			return;
		}

		_pm_service = new PowerMeterService(ble, 0, 0, 3200);

		ble.gattServer().onDataWritten(this, &PowerMeter::on_data_written);

		//print_mac_address();

		start_advertising();
	}

	void start_advertising()
	{
		/* Create advertising parameters and payload */

		ble::AdvertisingParameters adv_parameters(
		    ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
		    ble::adv_interval_t(ble::millisecond_t(1000)));

		_adv_data_builder.setFlags();
		_adv_data_builder.setLocalServiceList(mbed::make_Span(svc_uuids.data(), svc_uuids.size()));
		_adv_data_builder.setName(DEVICE_NAME);

		/* Setup advertising */

		auto& gap = ble.gap();
		ble_error_t error = gap.setAdvertisingParameters(
		    ble::LEGACY_ADVERTISING_HANDLE,
		    adv_parameters);

		if (error) {
			//printf("gap.setAdvertisingParameters() failed\r\n");
			return;
		}

		error = gap.setAdvertisingPayload(
		    ble::LEGACY_ADVERTISING_HANDLE,
		    _adv_data_builder.getAdvertisingData());

		if (error) {
			//printf("gap.setAdvertisingPayload() failed\r\n");
			return;
		}

		/* Start advertising */

		error = gap.startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);

		if (error) {
			//printf("gap.startAdvertising() failed\r\n");
			return;
		}
	}

	/**
	 * This callback allows the PowerMeterService to receive updates to the Characteristic.
	 *
	 * @param[in] params Information about the characterisitc being updated.
	 */
	void on_data_written(const GattWriteCallbackParams* params)
	{
		//if ((params->handle == _led_service->getValueHandle()) && (params->len == 1)) {
		//	_actuated_led = *(params->data);
		//}
	}

    private:
	/* Event handler */

	void onDisconnectionComplete(const ble::DisconnectionCompleteEvent&)
	{
		ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
	}

    private:
	BLE& ble;
	events::EventQueue& evq;

	const std::array<UUID, 1> svc_uuids { { PowerMeterService::UUID_POWER_METER_SERVICE } };

	PowerMeterService* _pm_service;

	uint8_t _adv_buffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
	ble::AdvertisingDataBuilder _adv_data_builder;
};

void sched_ble_events(BLE::OnEventsToProcessCallbackContext* context)
{
	event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}

int main()
{
	led.setColor(RGBLed::BLACK);

	bme.initialize();

	auto& ble = BLE::Instance();
	ble.onEventsToProcess(sched_ble_events);

	event_queue.call_every(1000, next_color);
	event_queue.call_every(1000, read_sensor);

	PowerMeter pm(ble, event_queue);
	pm.start();

	return 0;
}
