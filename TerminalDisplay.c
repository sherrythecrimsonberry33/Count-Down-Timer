/*
 * File:   TerminalDisplay.c
 * Author: Sheharyar
 *
 */


#include "TerminalDisplay.h"
#include "UART2.h"


void Display_init(void) {
    InitUART2();
}

void Display_clear(void) {
    XmitUART2('\r', 1);
    for(int i = 0; i < 30; i++) {
        XmitUART2(' ', 1);
    }
    XmitUART2('\r', 1);
}

void Display_update(Timer* timer) {
    Display_clear();
    Disp2String("SET ");
    Disp2Dec(timer->minutes);
    Disp2String("m : ");
    Disp2Dec(timer->seconds);
    Disp2String("s");
}

void Display_alarm(void) {
    Display_clear();
    Disp2String("FIN 00m : 00s -- ALARM");
}

void Display_reset(void) {
    Display_clear();
    Disp2String("CLR 00m : 00s");
}

void Display_count(Timer* timer) {
    Display_clear();
    Disp2String("CNT ");
    Disp2Dec(timer->minutes);
    Disp2String("m : ");
    Disp2Dec(timer->seconds);
    Disp2String("s");
}
