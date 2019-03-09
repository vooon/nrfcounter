/**
 * LED Blink counter.
 *
 * Each N-th blink means consumption of one kW*h.
 *
 * @author Vladimir Ermakov <vooon341@gmail.com>
 * @copyright 2019
 */

#include "mbed.h"
#include "mbed_events.h"
#include <stdio.h>

DigitalOut led_r(P0_27), led_g(P0_28), led_b(P0_29);

void blink(DigitalOut* gpio)
{
    *gpio = !gpio->read();
}

int main()
{
    // creates a queue with the default size
    EventQueue queue;

    // events are simple callbacks
    //queue.call(printf, "called immediately\n");
    //queue.call_in(2000, printf, "called in 2 seconds\n");
    //queue.call_every(1000, printf, "called every 1 seconds\n");

    queue.call_every(10, blink, &led_r);
    queue.call_every(20, blink, &led_g);
    queue.call_every(30, blink, &led_b);

    // events are executed by the dispatch method
    queue.dispatch();
}
