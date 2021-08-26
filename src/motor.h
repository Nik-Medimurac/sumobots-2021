// ============================================================================
// Sumobots 2021: Big Duck Energy
// ============================================================================
//
// Motor Controller Module
//
// Created By: Nikola Medimurac
// ============================================================================

#ifndef MOTOR_H
#define MOTOR_H

#include <avr/io.h>

#define LEFT_MOTOR_PIN  (0x1 << PORTD5) // Pin D5
#define RIGHT_MOTOR_PIN (0x1 << PORTD6) // Pin D6

/*
 * Initialises the motor controller module.
 * This sets up the LEFT_MOTOR_PIN and RIGHT_MOTOR_PIN as pwm outputs.
 */
void motor_init(void);

/*
 * Sets the power of the left motor.
 * Power value is between 0 - 255, where 0 is off and 255 is max power.
 */
static inline void  motor_left_power(unsigned char power) {
    OCR0A = power;
}

/*
 * Sets the power of the right motor.
 * Power value is between 0 - 255, where 0 is off and 255 is max power.
 */
static inline void  motor_right_power(unsigned char power) {
    OCR0B = power;
}

#endif /* MOTOR_H */
