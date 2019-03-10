/**
 * LED Blink counter.
 *
 * Each N-th blink means consumption of one kW*h.
 *
 * @author Vladimir Ermakov <vooon341@gmail.com>
 * @copyright 2019
 */

#include "RGBLed.h"
#include "mbed.h"
#include "mbed_events.h"
#include <array>
#include <stdio.h>

// Leads are inverted, active LOW
RGBLed led(P0_27, P0_28, P0_29);
// I2C's
I2C i2c_ap_bme(P0_19, P0_21);
I2C i2c_bmi_bmp(P0_11, P0_12);

// Queue
EventQueue event_queue;


void next_color()
{
	const std::array<RGBLed::Color, 8> colors{{
		RGBLed::BLACK,
		RGBLed::RED,
		RGBLed::GREEN,
		RGBLed::BLUE,
		RGBLed::MAGENTA,
		RGBLed::CYAN,
		RGBLed::YELLOW,
		RGBLed::WHITE
	}};

	static auto it = colors.cbegin();

	auto color = *it;
	led.setColor(color);

	if (++it == colors.cend())
		it = colors.cbegin();
}

int main()
{
	led.setColor(RGBLed::BLACK);

	// events are simple callbacks
	// queue.call(printf, "called immediately\n");
	// queue.call_in(2000, printf, "called in 2 seconds\n");
	// queue.call_every(1000, printf, "called every 1 seconds\n");

	event_queue.call_every(1000, next_color);

	event_queue.dispatch();
}
