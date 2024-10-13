/*
File:   TimeInterval.c (submit)
Author: Sheharyar, Aryan Mogera, Gerardo Garcia de Leon
*/

#include "xc.h"
#include "clkChange.h"

volatile uint16_t ms_counter = 0;
volatile uint8_t led_state = 0;
volatile uint16_t blink_interval = 0;

void initTimer1(void) {
    T1CONbits.TON = 0;      
    T1CONbits.TCS = 0;     
    T1CONbits.TCKPS = 1; // Select 1:8 Prescaler
    TMR1 = 0x00;            // Clear timer register
    
    // Calculate PR1  at 500kHz (calculations will be in the report)
    PR1 = 31;  
    
    IPC0bits.T1IP = 4;      // Set Timer1 interrupt priority level
    IFS0bits.T1IF = 0;      // Clear Timer1 interrupt flag
    IEC0bits.T1IE = 1;      // Enable Timer1 interrupt
    
    T1CONbits.TON = 1;      // Start Timer1
}

void setBlinkInterval(uint16_t interval_ms) {
    blink_interval = interval_ms;
    ms_counter = 0;
    led_state = 1;  // Start with LED on
    LATBbits.LATB8 = 1;
}

void stopBlinking(void) {
    blink_interval = 0;
    LATBbits.LATB8 = 0;  // Turn off LED
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0;  // Clear Timer_1 interrupt flag
    
    if (blink_interval > 0) {
        ms_counter++;
        if (ms_counter >= blink_interval) {
            ms_counter = 0;
            led_state = !led_state;
            LATBbits.LATB8 = led_state;
        }
    }
}


