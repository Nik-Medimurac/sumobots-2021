#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define LED_PIN (1 << PORTB5)

int main(void) {
    // Set pin direction to OUTPUT
    DDRB |= LED_PIN;

    // Flash LED on and off with a frequency of 1Hz
    while (true) {
        PORTB |= LED_PIN;       // Set pin HIGH
        _delay_ms(500);

        PORTB &= ~LED_PIN;      // Set pin LOW
        _delay_ms(500);
    }

    return 0;
}
