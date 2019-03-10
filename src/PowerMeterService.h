
#pragma once

#include <array>

#include "ble/BLE.h"
#include "ble/UUID.h"

class PowerMeterService {
    public:
	const UUID UUID_CHAR_PRESENTATION_FORMAT = 0x2904;

	static const UUID UUID_POWER_METER_SERVICE; // "caf92d82-4353-11e9-949b-4ccc6ad67d53";
	const UUID UUID_POWER_COUNTER_CHAR = 0xb001;
	const UUID UUID_PULSE_COUNTER_CHAR = 0xb002;
	const UUID UUID_PULSES_PER_KWH_CHAR = 0xb003;

	PowerMeterService(BLEDevice& _ble, uint32_t init_power_ctr, uint32_t init_pulse_ctr, uint16_t init_pulse_per_kwh)
	    : ble(_ble)
	    //, power_counter(UUID_POWER_COUNTER_CHAR, &init_power_ctr, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY, power_attrs.data(), power_attrs.size())
	    //, pulse_counter(UUID_PULSE_COUNTER_CHAR, &init_pulse_ctr, 0, pulse_attrs.data(), pulse_attrs.size())
	    //, pulses_per_kwh(UUID_PULSES_PER_KWH_CHAR, &init_pulse_per_kwh, 0, pulses_per_kwh_attrs.data(), pulses_per_kwh_attrs.size())
	    , power_counter(UUID_POWER_COUNTER_CHAR, &init_power_ctr, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY)
	    , pulse_counter(UUID_PULSE_COUNTER_CHAR, &init_pulse_ctr, 0)
	    , pulses_per_kwh(UUID_PULSES_PER_KWH_CHAR, &init_pulse_per_kwh, 0)

	{
		std::array<GattCharacteristic*, 3> char_table { { &power_counter, &pulse_counter, &pulses_per_kwh } };

		GattService service(UUID_POWER_METER_SERVICE, char_table.data(), char_table.size());

		ble.gattServer().addService(service);
	}

    private:
	BLEDevice& ble;

	ReadWriteGattCharacteristic<uint32_t> power_counter;
	ReadWriteGattCharacteristic<uint32_t> pulse_counter;
	ReadWriteGattCharacteristic<uint16_t> pulses_per_kwh;

	GattCharacteristic::PresentationFormat_t power_presentation {
		.gatt_format = GattCharacteristic::BLE_GATT_FORMAT_UINT32,
		.exponent = -3,
		.gatt_unit = GattCharacteristic::BLE_GATT_UNIT_ENERGY_KILOWATT_HOUR,
		.gatt_namespace = 0x01,
		.gatt_nsdesc = 0x0000
	};

	GattCharacteristic::PresentationFormat_t pulse_presentation {
		.gatt_format = GattCharacteristic::BLE_GATT_FORMAT_UINT32,
		.exponent = 0,
		.gatt_unit = GattCharacteristic::BLE_GATT_UNIT_NONE,
		.gatt_namespace = 0x01,
		.gatt_nsdesc = 0x0000,

	};

	GattCharacteristic::PresentationFormat_t pulses_per_kwh_presentation {
		.gatt_format = GattCharacteristic::BLE_GATT_FORMAT_UINT16,
		.exponent = 0,
		.gatt_unit = GattCharacteristic::BLE_GATT_UNIT_NONE,
		.gatt_namespace = 0x01,
		.gatt_nsdesc = 0x0000,

	};

	static constexpr size_t PF_SIZE = sizeof(GattCharacteristic::PresentationFormat_t);

	GattAttribute power_pr_attr { UUID_CHAR_PRESENTATION_FORMAT, reinterpret_cast<uint8_t*>(&power_presentation), PF_SIZE, PF_SIZE };
	GattAttribute pulse_pr_attr { UUID_CHAR_PRESENTATION_FORMAT, reinterpret_cast<uint8_t*>(&pulse_presentation), PF_SIZE, PF_SIZE };
	GattAttribute pulses_per_kwh_pr_attr { UUID_CHAR_PRESENTATION_FORMAT, reinterpret_cast<uint8_t*>(&pulses_per_kwh_presentation), PF_SIZE, PF_SIZE };

	std::array<GattAttribute*, 1> power_attrs { { &power_pr_attr } };
	std::array<GattAttribute*, 1> pulse_attrs { { &pulse_pr_attr } };
	std::array<GattAttribute*, 1> pulses_per_kwh_attrs { { &pulses_per_kwh_pr_attr } };
};

const UUID PowerMeterService::UUID_POWER_METER_SERVICE { { "caf92d82-4353-11e9-949b-4ccc6ad67d53" } };
