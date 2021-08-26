#include <avr/io.h>
//#include <util/delay.h>
//#include <stdbool.h>
//#include <avr/interrupt.h>
//#include <stdio.h>

#include "line_sensor.h"

#define LED_PIN (1 << PORTB5)

int main(void) {
    // Set pin direction to OUTPUT
    DDRB |= LED_PIN;

    line_sensor_init();

    while (1) {
        if (line_sensor_read()) {
            PORTB |= LED_PIN;       // Set pin HIGH
        }
        else {
            PORTB &= ~LED_PIN;      // Set pin LOW
        }

    }

    return 0;
}
