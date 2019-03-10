
#pragma once

#include "ble/BLE.h"
#include "ble/UUID.h"

class PowerMeterService {
    public:
	const UUID UUID_POWER_METER_SERVICE = "caf92d82-4353-11e9-949b-4ccc6ad67d53";
	const UUID UUID_POWER_COUNTER_CHAR = 0xb001;
	const UUID UUID_PULSE_COUNTER_CHAR = 0xb002;
	const UUID UUID_PULSES_PER_KWH_CHAR = 0xb003;

	PowerMeterService(BLEDevice& _ble, uint32_t init_power_ctr, uint32_t init_pulse_ctr, uint16_t init_pulse_per_kwh)
	    : ble(_ble)
	    , power_counter(UUID_POWER_COUNTER_CHAR, &init_power_ctr, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY, nullptr, 0)
	    , pulse_counter(UUID_PULSE_COUNTER_CHAR, &init_pulse_ctr, 0, nullptr, 0)
	    , pulses_per_kwh(UUID_PULSES_PER_KWH_CHAR, &init_pulse_per_kwh, 0, nullptr, 0)
	{
	}

    private:
	BLEDevice& ble;

	ReadWriteGattCharacteristic<uint32_t> power_counter;
	ReadWriteGattCharacteristic<uint32_t> pulse_counter;
	ReadWriteGattCharacteristic<uint16_t> pulses_per_kwh;

	const GattCharacteristic::PresentationFormat_t power_presentation {
		.gatt_format = GattCharacteristic::BLE_GATT_FORMAT_UINT32,
		.exponent = -3,
		.gatt_unit = GattCharacteristic::BLE_GATT_UNIT_ENERGY_KILOWATT_HOUR,
		.gatt_namespace = 0x01,
		.gatt_nsdesc = 0x0000
	};

	const GattCharacteristic::PresentationFormat_t pulse_presentation {
		.gatt_format = GattCharacteristic::BLE_GATT_FORMAT_UINT32,
		.exponent = 0,
		.gatt_unit = GattCharacteristic::BLE_GATT_UNIT_NONE,
		.gatt_namespace = 0x01,
		.gatt_nsdesc = 0x0000,

	};

	const GattCharacteristic::PresentationFormat_t pulses_per_kwh_presentation {
		.gatt_format = GattCharacteristic::BLE_GATT_FORMAT_UINT16,
		.exponent = 0,
		.gatt_unit = GattCharacteristic::BLE_GATT_UNIT_NONE,
		.gatt_namespace = 0x01,
		.gatt_nsdesc = 0x0000,

	};

	//GattAttribute power_attrs[1];
	//GattAttribute pulse_attrs[1];
	//GattAttribute pulses_per_kwh_attrs[1];
};
