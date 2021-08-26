// ============================================================================
// Sumobots 2021: Big Duck Energy
// ============================================================================
//
// Line Sensor Module
//
// Created By: Nikola Medimurac
// ============================================================================

#include "line_sensor.h"

void line_sensor_init (void) {
    /* Set line sensor pin to be an input. This is the default setting but
     * will set it again in case it was changed somewhere else */
    DDRB &= ~LINE_SENSOR_PIN;
}
