/*
File:   TimeInterval.h (submit)
Author: Sheharyar, Aryan Mogera, Gerardo Garcia de Leon
*/

#ifndef TIMEDELAY_H
#define TIMEDELAY_H

#include <xc.h>

void initTimer1(void);
void setBlinkInterval(uint16_t interval_ms);
void stopBlinking(void);

#endif // TIMEDELAY_H