/*
File:   IOs.c  (submit)
Author: Sheharyar, Aryan Mogera, Gerardo Garcia de Leon
*/

#include "IOs.h"
#include "TimeInterval.h"
#include "clkChange.h"
#include "UART2.h"

typedef enum {
    STATE_IDLE,
    STATE_PB1,
    STATE_PB2,
    STATE_PB3,
    STATE_PB1_PB2,
    STATE_PB1_PB3,
    STATE_PB2_PB3,
    STATE_ALL_PB
} ButtonCurrState;

static ButtonCurrState currentState = STATE_IDLE;
static ButtonCurrState previousState = STATE_IDLE;

void IOinit(void) {
    AD1PCFG = 0xFFFF;  // Set all pins to digital mode

    // Configure  resistors
    CNPU1bits.CN0PUE = 1;  // PB3 
    CNPU1bits.CN1PUE = 1;  // PB2 
    CNPU2bits.CN30PUE = 1; // PB1 
     
    // Enable CN interrupts
    CNEN1bits.CN0IE = 1;   // PB3 
    CNEN1bits.CN1IE = 1;   // PB2 
    CNEN2bits.CN30IE = 1;  // PB1 
    
    IFS1bits.CNIF = 0;  // Clear CN interrupt flag
    IEC1bits.CNIE = 1;  // Enable CN interrupt
    IPC4bits.CNIP = 6;  // Set CN interrupt priority
    
    // Configure I/O
    TRISBbits.TRISB8 = 0;  
    LED = 0;               
    
    TRISAbits.TRISA2 = 1;  // PB1 as input
    TRISBbits.TRISB4 = 1;  // PB2 as input
    TRISAbits.TRISA4 = 1;  // PB3 as input

    InitUART2();
    initTimer1();  // Initialize Timer1 for LED blinking
}

void displayState(ButtonCurrState state) {
    Disp2String("\r                                        \r"); // Clear the line
    switch(state) {
        case STATE_IDLE:
            Disp2String("Nothing pressed");
            break;
        case STATE_PB1:
            Disp2String("PB1 is pressed");
            break;
        case STATE_PB2:
            Disp2String("PB2 is pressed");
            break;
        case STATE_PB3:
            Disp2String("PB3 is pressed");
            break;
        case STATE_PB1_PB2:
            Disp2String("PB1 and PB2 are pressed");
            break;
        case STATE_PB1_PB3:
            Disp2String("PB1 and PB3 are pressed");
            break;
        case STATE_PB2_PB3:
            Disp2String("PB2 and PB3 are pressed");
            break;
        case STATE_ALL_PB:
            Disp2String("All PBs pressed");
            break;
    }
}

void updateLEDState(ButtonCurrState state) {
    switch(state) {
        case STATE_PB1:
            setBlinkInterval(250);  // 0.25 sec intervals
            break;
        case STATE_PB2:
            setBlinkInterval(2000);  // 2 sec intervals
            break;
        case STATE_PB3:
            setBlinkInterval(4000);  // 4 sec intervals
            break;
        case STATE_PB1_PB2:
        case STATE_PB1_PB3:
        case STATE_PB2_PB3:
        case STATE_ALL_PB:
            stopBlinking();
            LED = 1;  // Keep LED on
            break;
        case STATE_IDLE:
            stopBlinking();
            LED = 0;  // Turn LED off
            break;
    }
}

void IOcheck(void) {
    ButtonCurrState newState;

    if (!PB1 && PB2 && PB3) newState = STATE_PB1;
    else if (PB1 && !PB2 && PB3) newState = STATE_PB2;
    else if (PB1 && PB2 && !PB3) newState = STATE_PB3;
    else if (!PB1 && !PB2 && PB3) newState = STATE_PB1_PB2;
    else if (!PB1 && PB2 && !PB3) newState = STATE_PB1_PB3;
    else if (PB1 && !PB2 && !PB3) newState = STATE_PB2_PB3;
    else if (!PB1 && !PB2 && !PB3) newState = STATE_ALL_PB;
    else newState = STATE_IDLE;

    if (newState != currentState) {
        previousState = currentState;
        currentState = newState;
        
        // Update LED state
        updateLEDState(currentState);
        displayState(currentState);
    }
}

void __attribute__((interrupt, auto_psv)) _CNInterrupt(void) {
    IFS1bits.CNIF = 0;  // Clear CN interrupt flag

}

