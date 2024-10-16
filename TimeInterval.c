/*
 * File:   TimeDelay.c
 * Author: Sheharyar
 */
// TimeDelay.c
#include "TimeInterval.h"
#include <xc.h>

// Macro for Idle mode
#define Idle() {__asm__ volatile ("pwrsav #1");}


int Delay_ms(uint16_t time_ms) {
    
    TMR1 = 0x00;            // Clear timer register
    
    T1CONbits.TCS = 0; 
    T1CONbits.TCKPS0 = 1;
    T1CONbits.TCKPS1 = 1;// Select 1:8 Prescaler
    T1CONbits.TON = 1;      // Start Timer1
    
     
    
    IPC0bits.T1IP = 6;      // Set Timer1 interrupt priority level
   
    IEC0bits.T1IE = 1;      // Enable Timer1 interrupt
    
    IFS0bits.T1IF = 0;      // Clear Timer1 interrupt flag
   
    PR1 = time_ms*16; 
    
    Idle();
   
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0;  // Clear Timer_1 interrupt flag
    
    T1CONbits.TON = 0;      // Stop Timer1

}
