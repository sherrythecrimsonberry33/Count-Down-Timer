/*
 * File:   TimerOperations.c
 * Author: Sheharyar
 *

 */


#include "TimerOperations.h"

void Timer_init(Timer* timer) {
    timer->minutes = 0;
    timer->seconds = 0;
    timer->state = STATE_IDLE;
}

void Timer_incrementMinutes(Timer* timer) {
    timer->minutes = (timer->minutes + 1) % 60;
}

void Timer_incrementSeconds(Timer* timer, int increment) {
    timer->seconds = (timer->seconds + increment) % 60;
}

void Timer_start(Timer* timer) {
    timer->state = STATE_RUNNING;
}

void Timer_pause(Timer* timer) {
    timer->state = STATE_PAUSED;
}

void Timer_reset(Timer* timer) {
    timer->minutes = 0;
    timer->seconds = 0;
    timer->state = STATE_IDLE;
}

void Timer_tick(Timer* timer) {
    if (timer->state != STATE_RUNNING) return;
    
    if (timer->seconds > 0) {
        timer->seconds--;
    } else if (timer->minutes > 0) {
        timer->minutes--;
        timer->seconds = 59;
    } else {
        timer->state = STATE_FINISHED;
    }
}

int Timer_isFinished(Timer* timer) {
    return timer->state == STATE_FINISHED;
}

int Timer_isRunning(Timer* timer) {
    return timer->state == STATE_RUNNING;
}
