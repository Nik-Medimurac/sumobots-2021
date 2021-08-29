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

/*
 * Initialises the motor controller module.
 * This configures the motor control pins as pwm outputs, with the motors
 * initially turned off.
 * */
void motor_init(void);

/*
 * Sets the power of the left motor.
 * Power value is between -127 to 127, where:
 *   -127 : Full power reverse
 *      0 : Off
 *    127 : Full power forward
 * */
void motor_left_power(char power);

/*
 * Sets the power of the right motor.
 * Power value is between -127 to 127, where:
 *   -127 : Full power reverse
 *      0 : Off
 *    127 : Full power forward
 * */
void motor_right_power(char power);

#endif /* MOTOR_H */
