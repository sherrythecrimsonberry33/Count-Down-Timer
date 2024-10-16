#ifndef TERMINAL_DISPLAY_H
#define TERMINAL_DISPLAY_H

#include "TimerOperations.h"

void Display_init(void);
void Display_update(Timer* timer);
void Display_alarm(void);
void Display_reset(void);
void Display_count(Timer* timer);


#endif // DISPLAY_H
