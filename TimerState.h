// TimerState.h
#ifndef TIMER_STATE_H
#define TIMER_STATE_H

typedef enum {
    STATE_IDLE,
    STATE_SET_MINUTES,
    STATE_SET_SECONDS,
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_FINISHED
} TimerState;

#endif // TIMER_STATE_H