// ============================================================================
// Sumobots 2021: Big Duck Energy
// ============================================================================
//
// Motor Controller Module
//
// Created By: Nikola Medimurac
// ============================================================================

#include "motor.h"

void motor_init(void) {
    /* Set the motor pins as outputs */
    DDRD |= LEFT_MOTOR_PIN | RIGHT_MOTOR_PIN;

    /* Configure motor pins for pwm output */
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00);
    TCCR0B = (1 << CS00);
}
