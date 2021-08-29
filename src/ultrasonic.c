// ============================================================================
// Sumobots 2021: Big Duck Energy
// ============================================================================
//
// Code related to using the ultrasonic sensor
//
// Created By: Nikola Medimurac
// ============================================================================

#include "ultrasonic.h"

#include <avr/io.h>
#include <avr/interrupt.h>

/* Define pins the ultrasonic sensors are connected to */
#define ECHO_PIN_US1 (0x1 << PORTD2) // Pin D2
#define ECHO_PIN_US2 (0x1 << PORTB4) // Pin D12
#define TRIG_PIN_US1 (0x1 << PORTD4) // Pin D4
#define TRIG_PIN_US2 (0x1 << PORTD7) // Pin D7

/* Define the counter value that with prescalar x1, will timeout in 10us */
#define TEN_US_COUNTER 160

/* States the ultrasonic pulsing process can reaches */
enum ultrasonic_state {
    STANDBY,   // Standby waiting for an async read request
    US1_SEND, // Sending the pulse from ultrasonic sensor 1 out
    US1_WAIT, // Waiting for pulse from ultrasonic sensor 1 to return
    US2_SEND, // Sending the pulse from ultrasonic sensor 2 out
    US2_WAIT, // Waiting for pulse from ultrasonic sensor 2 to return
};

/* Global variable for tracking ultrasonic pulsing data */
static struct {
    /* Store the results obtained from ultrasonic sensors during read process */
    unsigned int us1_result;
    unsigned int us2_result;
    /* Callback function to invoke after reading process complete */
    void (*callback_func)(unsigned int, unsigned int);
    /* The state the ultrasonic process is currently in */
    enum ultrasonic_state state;
    /* Number of times PCINT0 interrupt handler
     * has been called during current read process */
    unsigned char pcint0_times;
} ultrasonic;

void ultrasonic_init()
{
    /* Set trigger pin of both ultrasonic sensors to be an output */
    DDRD |= (TRIG_PIN_US1 | TRIG_PIN_US2);

    /* Echo pin of ultrasonic sensor 1 is connected to external interrupt 0.
     * Configure this interupt to be falling edge triggered */
    DDRD &= ~(ECHO_PIN_US1);
    EICRA |= (1 << ISC01);

    /* Echo pin of ultrasonic sensor 2 is connected to PCINT4. This is
     * configured to be an external interrupt. Didnt use external interrupt 1
     * because this is used for pwm with Timer2 */
    DDRB &= ~(ECHO_PIN_US2);
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT4);

    /* Configure Timer1 to be in compare A interrupt mode. 
     * Timer1 wont actually be started until async read is called */
    TCCR1B &= ~((1 << CS10) | (1 << CS11) |(1 << CS12));
    TCNT1 = 0;
    OCR1A = 1;
    TIMSK1 |= (1 << OCIE1A);
    
    /* Start in state STANDBY and initialise other variables */ 
    ultrasonic.state = STANDBY;
    ultrasonic.us1_result = 0;
    ultrasonic.us2_result = 0;
    ultrasonic.callback_func = 0;
}

/* 
 * Function to start sending a pulse from ultrasonic sensor out.
 * Sets trigger pin high and configures Timer1 to fire interrupt in 10us. The
 * trigger pin should then be set low in the interrupt handler.
 * */
static void start_pulse_send(unsigned char us_number)
{
    /* Set no prescalar for Timer1 to stop timer */
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));

    /* Set the given trigger pin high */
    if (us_number == 1) {
        PORTD |= TRIG_PIN_US1;
    }
    else {
        PORTD |= TRIG_PIN_US2;
    }

    /* Set timer values interrupt fires in 10us */
    TCNT1 = 0;
    OCR1A = TEN_US_COUNTER;

    /* Set prescalar x1 to start timer */
    TCCR1B |= (1 << CS10);
}

/*
 * Call this after pulse is sent, it sets up the process of reading the result
 * from the sent pulse. This involves reseting the value in Timer1, and setting
 * it to fire an interrupt at the the set rate if the pulse doesnt return in
 * time. 
 * */
static void start_pulse_read(unsigned char us_number)
{
    /* Set no prescalar for Timer1 to stop timer */
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));

    /* Set Timer1 values so the interrupt fires at the desired rate */
    TCNT1 = 0;
    OCR1A = ULTRASONIC_TIMEOUT;

    /* Enable external interrupt for the echo pin of the
     * ultrasonic sensor being pulsed */
    if (us_number == 1) {
        EIMSK |= (1 << INT0);
    }
    else {
        PCICR |= (1 << PCIE0);
    }

    /* Set prescalar x8 to start timer */
    TCCR1B |= (1 << CS11);
}


int ultrasonic_async_read(void (*callback_func)(unsigned int, unsigned int))
{
    /* Return -1 if there is already a read operation in progress */
    if (ultrasonic.state != STANDBY) {
        return -1;
    }

    /* Store callback function so it can be called after read process done */
    ultrasonic.callback_func = callback_func;

    /* Intialise results as max value (timeout). This is the default return 
     * value if a signal does not return back to ultrasonic sensor in time */
    ultrasonic.us1_result = ULTRASONIC_TIMEOUT;
    ultrasonic.us2_result = ULTRASONIC_TIMEOUT;

    /* Set pcint0_times as 0 since it has not been called yet on this read */
    ultrasonic.pcint0_times = 0;

    /* Start sending pulse from ultrasonic sensor 1, and update state.
     * Rest of reading process will continue in background */
    start_pulse_send(1);
    ultrasonic.state = US1_SEND;

    return 0;
}

/* 
 * Timer1 compare A interrupt handler 
 * Called when the timer set expires
 * */
ISR(TIMER1_COMPA_vect) {
    /* Do something depending on the state currently in */
    switch (ultrasonic.state) {
        case US1_SEND:
            /* If timer expired after sending pulse. Set trigger pin low
             * and setup to read the returning signal. */
            PORTD &= ~TRIG_PIN_US1;
            start_pulse_read(1);
            ultrasonic.state = US1_WAIT;
            break;

        case US1_WAIT:
            /* If timer expired after waiting for pulse. Then the pulse took
             * too long to come back, ignore it and move to pulsing next 
             * ultrasonic sensor */
            start_pulse_send(2);
            ultrasonic.state = US2_SEND;
            break;

        case US2_SEND:
            /* If timer expired after sending pulse. Set trigger pin low
             * and setup to read the returning signal. */
            PORTD &= ~TRIG_PIN_US2;
            start_pulse_read(2);
            ultrasonic.state = US2_WAIT;
            break;
        
        case US2_WAIT:
            /* If timer expired after waiting for pulse. Then the pulse took
             * too long to come back. Done pulsing both ultrasonic sensors, 
             * so stop timer and call callback function with result. */
            TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
            ultrasonic.state = STANDBY;
            ultrasonic.callback_func(ultrasonic.us1_result, ultrasonic.us2_result);
            break;

        default:
            /* This shouldn't ever happened. If it does for some reason
             * set state back to STANDBY and stop timer to go to a safe state */
            TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
            ultrasonic.state = STANDBY;
    }
}

/* 
 * External interrupt 0 handler.
 * Called when echo pin of ultrasonic sensor 1 goes high.
 * */
ISR(INT0_vect) {
    /* This could be called when no pulse was sent out due to noise,
     * only do anything if in the state where waiting for the pulse */
    if (ultrasonic.state == US1_WAIT) {
        /* Stop Timer1 and clear the Timer1 Compare A interrupt flag.
         * This is to ensure the handler isnt called as the action
         * to be taken is handled here */
        TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
        TIFR1  &= ~(1 << OCF1A);

        /* Save value in Timer1 counter and disable this interrupt */
        ultrasonic.us1_result = TCNT1;
        EIMSK &= ~(1 << INT0);
    }

    /* Start pulsing ultrasonic sensor 2 */
    start_pulse_send(2);
    ultrasonic.state = US2_SEND;
}

ISR(PCINT0_vect) {
    /* This could be called when no pulse was sent out due to noise,
     * only do anything if in the state where waiting for the pulse */
    if (ultrasonic.state == US2_WAIT) {
        /* Increment the number of times this interrupt handler has been called */
        ultrasonic.pcint0_times = ultrasonic.pcint0_times + 1;

        /* If it is 2nd time being called, handle the interrupt
         * This is because this interrupt is edge triggered, and we want
         * only the falling edge. (1st time is the rising edge) */
        if (ultrasonic.pcint0_times == 2) {
            /* Stop Timer1 and clear the Timer1 Compare A interrupt flag.
             * This is to ensure the handler isnt called as the action
             * to be taken is handled here */
            TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
            TIFR1  &= ~(1 << OCF1A);

            /* Save value in Timer1 counter and disable this interrupt */
            ultrasonic.us2_result = TCNT1;
            PCICR &= ~(1 << PCIE0);

            /* Done pulsing both ultrasonic sensors,
             * call callback function with result. */
            ultrasonic.state = STANDBY;
            ultrasonic.callback_func(ultrasonic.us1_result, ultrasonic.us2_result);
        }
    }
}
