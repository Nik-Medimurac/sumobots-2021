// ============================================================================
// Sumobots 2021: Big Duck Energy
// ============================================================================
//
// Motor Controller Module
//
// Created By: Nikola Medimurac
// ============================================================================

#include "motor.h"

#include <avr/io.h>

#define MOTOR_PIN_L_F (0x1 << PORTD6) // Pin D6, left motor forward control pin
#define MOTOR_PIN_L_R (0x1 << PORTD5) // Pin D5, left motor reverse control pin
#define MOTOR_PIN_R_F (0x1 << PORTB3) // Pin D11,right motor forward control pin
#define MOTOR_PIN_R_R (0x1 << PORTD3) // Pin D3, right motor reverse control pin

void motor_init(void)
{
    /* Set the motor pins as outputs */
    DDRD |= MOTOR_PIN_L_F | MOTOR_PIN_L_R | MOTOR_PIN_R_R;
    DDRB |= MOTOR_PIN_R_F;

    /* Configure left motor pins for pwm output using Timer0 */
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00);
    TCCR0B = (1 << CS00);

    /* Configure right motor pins for pwm output using Timer2 */
    TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM20);
    TCCR2B = (1 << CS20);
}

void motor_left_power(char power)
{
    /* Set duty cycle on forward and reverse pin to get desired power. This is a
     * 8bit value so need to double it, and in the case of reverse, flip sign */
    if (power < 0) {
        OCR0A = 0;
        OCR0B = power * -2;
    }
    else if (power > 0) {
        OCR0A = power * 2;
        OCR0B = 0;
    }
    else {
        OCR0A = 0;
        OCR0B = 0;
    }
}

void motor_right_power(char power)
{
    /* Set duty cycle on forward and reverse pin to get desired power. This is a
     * 8bit value so need to double it, and in the case of reverse, flip sign */
    if (power < 0) {
        OCR2A = 0;
        OCR2B = power * -2;
    }
    else if (power > 0) {
        OCR2A = power * 2;
        OCR2B = 0;
    }
    else {
        OCR2A = 0;
        OCR2B = 0;
    }
}
