// ============================================================================
// Sumobots 2021: Big Duck Energy
// ============================================================================
//
// Ultrasonic Sensor Module
//
// Created By: Nikola Medimurac
// ============================================================================

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

/* 
 * Define the rate ultrasonic sensors are pulsed at.
 * For every 1, there is close to a 16us delay between pulsing a ultrasonic,
 * or 32us before the same ultrasonic is pulsed again.
 * */
//#define ULTRASONIC_PULSE_RATE 31000
#define ULTRASONIC_TIMEOUT 60000

/*
 * Initialises the ultrasonic sensor module.
 * Pulsing of ultrasonic sensors starts in the background after this function.
 * Takes in a pointer to 2 double values, the distance measured by each
 * ultrasonic is saved to this value, use these values to get the distances.
 * */
void ultrasonic_init();

int ultrasonic_async_read(void (*callback_func)(unsigned int, unsigned int));

#endif /* ULTRASONIC_H */
