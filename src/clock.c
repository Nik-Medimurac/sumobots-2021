// ============================================================================
// Sumobots 2021: Big Duck Energy
// ============================================================================
//
// Clock Module
//
// Created By: Nikola Medimurac
// ============================================================================

#include "clock.h"

#include <avr/io.h>
#include <avr/interrupt.h>

static struct {
    /* Number of senconds passed since clock was started */
    unsigned char time_seconds;
    /* Number of milliseconds passed since last full second */
    unsigned int milliseconds;
} clock;

void clock_start(void)
{
    /* Initialise time to 0 seconds */
    clock.time_seconds = 0;
    clock.milliseconds = 0;

    /* Set prescalar to x64 and CTC mode for Timer0 */
    TCCR0A |= (1 << WGM01);
    TCCR0B |= (1 << CS01) | (1 << CS00);

    /* Enable compare A match interrupt and set OCR0! so that
     * the interrupt fires after 1 millisecond */
    TIMSK0 |= (1 << OCIE0A);
    OCR0A = 250;
}

void clock_stop(void)
{
    /* Write all 0s to TCCR0B and TCCRoA to reset Timer0 to inital state */
    TCCR0A = 0;
    TCCR0B = 0;
}

unsigned char clock_read(void)
{
    return clock.time_seconds;
}

/* 
 * Interrupt handler for output compare A, this is called every 1 millisecond 
 * */
ISR(TIMER0_COMPA_vect)
{
    /* Increment milliseoncds counter. If 1000 milliseconds pass, reset the
     * millisecond counter and increment the second counter */
    clock.milliseconds = clock.milliseconds + 1;
    if (clock.milliseconds >= 1000) {
        clock.time_seconds = clock.time_seconds + 1;
        clock.milliseconds = 0;
    }
}
