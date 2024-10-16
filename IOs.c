/*
 * File:   IOs.c
 * Author: Sheharyar
 *
 */
// IOControl.c
#include "IOs.h"
#include "TimeInterval.h"
#include "TerminalDisplay.h"
#include <xc.h>


#define FCY 8000000UL  // 8 MHz clock frequency
#define CLOCK_FREQ FCY
#define PB1 PORTAbits.RA2
#define PB2 PORTAbits.RA4
#define PB3 PORTBbits.RB4
#define LED LATBbits.LATB8

static int pb2_hold_time = 0;
//static int pb3_press_duration = 0;
//static int pb3_prev_state = 1;
static Timer* global_timer;

static unsigned long timer_ms = 0;
static int pb3_pressed = 0;
static unsigned long pb3_press_start = 0;
static const unsigned long pb3_long_press_threshold = 3000; // 3 seconds



void IOControl_init(Timer* timer) {
    global_timer = timer;

    AD1PCFG = 0xFFFF;
    
    CNPU1bits.CN0PUE = 1;
    CNPU1bits.CN1PUE = 1;
    CNPU2bits.CN30PUE = 1;
    
    CNEN1bits.CN0IE = 1;
    CNEN1bits.CN1IE = 1;
    CNEN2bits.CN30IE = 1;
    
    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;
    
    TRISBbits.TRISB8 = 0;
    LED = 0;
    
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA4 = 1;
    TRISBbits.TRISB4 = 1;
}

void IOControl_check(Timer* timer) {
    if (timer->state == STATE_IDLE || timer->state == STATE_PAUSED) {
        if (!PB1) {
            Timer_incrementMinutes(timer);
            Display_update(timer);
            Delay_ms(300);
        }
        
        if (!PB2) {
            pb2_hold_time += 300;
            int increment = (pb2_hold_time >= 3000) ? 5 : 1;
            Timer_incrementSeconds(timer, increment);
            Display_update(timer);
            Delay_ms(300);
        } else {
            pb2_hold_time = 0;
        }
    }
    
  if (Timer_isRunning(timer)) {
        LED ^= 1;
        Timer_tick(timer);
        if (Timer_isFinished(timer)) {
            Display_alarm();
            LED = 1;
        } else {
            Display_count(timer);
        }
        Delay_ms(1000);
    } else {
        LED = 0;
    }

    // Increment our timer
    timer_ms += 10;  // Assuming this function is called every 10ms

    // Check for PB3 press and release
    if (!PB3) {
        if (!pb3_pressed) {
            pb3_pressed = 1;
            pb3_press_start = timer_ms;
        }
    } else {
        if (pb3_pressed) {
            pb3_pressed = 0;
            unsigned long press_duration = timer_ms - pb3_press_start;

            if (press_duration >= pb3_long_press_threshold) {
                Timer_reset(timer);
                Display_reset();
            } else {
                if (Timer_isRunning(timer)) {
                    Timer_pause(timer);
                    Display_update(timer);
                } else {
                    Timer_start(timer);
                    Display_count(timer);
                }
            }
        }
    }
}
void __attribute__((interrupt, auto_psv)) _CNInterrupt(void) {
    IFS1bits.CNIF = 0;
    

}
