// ============================================================================
// Sumobots 2021: Big Duck Energy
// ============================================================================
//
// Clock Module
//
// Created By: Nikola Medimurac
// ============================================================================

#ifndef CLOCK_H
#define CLOCK_H

/* 
 * Starts the clock module
 * */
void clock_start(void);

/*
 * Stops the clock module, resetting all altered control registers 
 * */
void clock_stop(void);

/*
 * Returns the number of seconds elapsed since this clock with started 
 * */
unsigned char clock_read(void);

#endif /* CLOCK_H */
