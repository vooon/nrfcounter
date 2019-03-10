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

// Leads are inverted, active LOW
RGBLed led(P0_27, P0_28, P0_29);
// I2C's
I2C i2c_ap_bme(P0_19, P0_21);
I2C i2c_bmi_bmp(P0_11, P0_12);

BME280 bme(i2c_ap_bme);

const char DEVICE_NAME[] = "NRF-COUNTER";
const uint16_t uuid16_list[] = { GattService::UUID_ENVIRONMENTAL_SERVICE };

// Queue
EventQueue event_queue;
EnvironmentalService* penvsrv = nullptr;

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

void disconnection_cb(const Gap::DisconnectionCallbackParams_t* params)
{
	// restart advertising
	BLE::Instance().gap().startAdvertising();
}

void ble_init_complete(BLE::InitializationCompleteCallbackContext* params)
{
	BLE& ble = params->ble;
	auto& gap = ble.gap();

	auto error = params->error;

	penvsrv = new EnvironmentalService(ble);
	gap.onDisconnection(disconnection_cb);

	// Setup advertising
	gap.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
	gap.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, uuid16_list, sizeof(uuid16_list));
	gap.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, DEVICE_NAME, sizeof(DEVICE_NAME));
	gap.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
	gap.setAdvertisingInterval(1000);

	error = gap.startAdvertising();
}

void read_sensor()
{
	float t, p, h;

	if (penvsrv == nullptr)
		return;

	t = bme.getTemperature();
	p = bme.getPressure();
	h = bme.getHumidity();

	penvsrv->updateTemperature(t);
	penvsrv->updatePressure(p);
	penvsrv->updateHumidity(h);
}

void sched_ble_events_processing(BLE::OnEventsToProcessCallbackContext* context)
{
	auto& ble = BLE::Instance();
	event_queue.call(Callback<void()>(&ble, &BLE::processEvents));
}

int main()
{
	led.setColor(RGBLed::BLACK);

	auto& ble = BLE::Instance();
	ble.onEventsToProcess(sched_ble_events_processing);
	ble.init(ble_init_complete);

	event_queue.call_every(1000, next_color);
	event_queue.call_every(1000, read_sensor);

	event_queue.dispatch_forever();

	return 0;
}
