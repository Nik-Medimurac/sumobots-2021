// ============================================================================
// Sumobots 2021: Big Duck Energy
// ============================================================================
//
// Line Sensor Module
//
// Created By: Nikola Medimurac
// ============================================================================

#ifndef LINE_SENSOR_H
#define LINE_SENSOR_H

#include <avr/io.h>

#define LINE_SENSOR_PIN (0x1 << PORTD7)

/*
 * Initialises the line sensor module.
 * LINE_SENSOR_PIN is set to be the input pin of the line sensor.
 */
void line_sensor_init (void);

/*
 * Returns:
 *     >0 : The ground beneath the line sensor is considered bright.
 *      0 : The ground beneath the line sensor is considered dark.
 */
static inline int line_sensor_read (void) {
    return (PIND & LINE_SENSOR_PIN);
}

#endif /* LINE_SENSOR_H */
