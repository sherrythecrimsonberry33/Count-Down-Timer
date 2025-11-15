#ifndef TIMER_OPERATIONS_H
#define TIMER_OPERATIONS_H

#include "TimerState.h"

typedef struct {
    int minutes;
    int seconds;
    TimerState state;
} Timer;

void Timer_init(Timer* timer);
void Timer_incrementMinutes(Timer* timer);
void Timer_incrementSeconds(Timer* timer, int increment);
void Timer_start(Timer* timer);
void Timer_pause(Timer* timer);
void Timer_reset(Timer* timer);
void Timer_tick(Timer* timer);
int Timer_isFinished(Timer* timer);
int Timer_isRunning(Timer* timer);

#endif // TIMER_OPERATIONS_H