#ifndef __DUE_TIMER_C_WRAPPER_H
#define __DUE_TIMER_C_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif 

struct DueTimer; 
typedef struct DueTimer DueTimer_t;

DueTimer_t * NewTimer(int index);
DueTimer_t * TimerAttachInterrupt(DueTimer_t * timer, void * isr);
DueTimer_t * TimerDetachInterrupt(DueTimer_t * timer);
DueTimer_t * TimerStart(DueTimer_t * timer, long microseconds);
DueTimer_t * TimerStop(DueTimer_t * timer);
DueTimer_t * TimerSetFrequency(DueTimer_t * timer, double frequency);
DueTimer_t * TimerSetPeriod(DueTimer_t * timer, unsigned long microseconds);

double TimerGetFrequency(DueTimer_t * timer);
long TimerGetPeriod(DueTimer_t * timer);

#ifdef __cplusplus
}
#endif
#endif 