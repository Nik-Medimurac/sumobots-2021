//#include <avr/io.h>
#include <util/delay.h>
//#include <stdbool.h>
//#include <avr/interrupt.h>
//#include <stdio.h>

#include "line_sensor.h"
#include "motor.h"

#define LED_PIN (1 << PORTB5)

int main(void) {
    // Set pin direction to OUTPUT
    DDRB |= LED_PIN;
    char i = 0;

    line_sensor_init();
    motor_init();

    motor_left_power(255);
    while (1) {
        if (line_sensor_read()) {
            motor_right_power(i);
            i = i + 1;
            _delay_ms(100);
            PORTB |= LED_PIN;       // Set pin HIGH
        }
        else {
            i = 0;
            motor_right_power(i);
            PORTB &= ~LED_PIN;      // Set pin LOW
        }

    }

    return 0;
}
