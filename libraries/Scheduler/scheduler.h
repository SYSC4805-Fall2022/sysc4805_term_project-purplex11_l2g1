/****************************************************************************************
 * Filename: scheduler.h
 * Author: Justin Whalley
 * Description: Header file to contain structs and function declarations for scheduler module
****************************************************************************************/

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <DueTimer.h>
#include <cmath>

#define MAX_HANDLERS_PER_TIMER      10

#define MAX_TIMERS                  9

#define TIMER0_RESERVED
#define TIMER1_RESERVED
#define TIMER6_RESERVED

#define TIMER3_ENABLE
#define TIMER3_MS                   50

#define NO_ERROR                    1
#define NO_TIMERS_ERROR             2
#define HANDLER_FULL_ERROR          3
#define UNKNOWN_ERROR               4


class Handler
{
public:
    uint16_t timerTicks;
    uint16_t waitTimerTicks;
    uint8_t  timerIndex;
    uint8_t  hasISR;
    void (*isr)();

    Handler ()
    {
        timerTicks=0;
        waitTimerTicks=0;
        timerIndex=0;
        hasISR=0;

    };

    void Run();
    void Start(void (*setISR)(), uint16_t setWaitTimerTicks, uint8_t setTimerIndex);
};

class TimerScheduler
{
public:

#ifdef TIMER0_ENABLE
    Handler Timer0Handlers[MAX_HANDLERS_PER_TIMER]; 
#endif
#ifdef TIMER1_ENABLE
    Handler Timer1Handlers[MAX_HANDLERS_PER_TIMER]; 
#endif
#ifdef TIMER2_ENABLE
    Handler Timer2Handlers[MAX_HANDLERS_PER_TIMER]; 
#endif
#ifdef TIMER3_ENABLE
    Handler Timer3Handlers[MAX_HANDLERS_PER_TIMER]; 
#endif
#ifdef TIMER4_ENABLE
    Handler Timer4Handlers[MAX_HANDLERS_PER_TIMER]; 
#endif
#ifdef TIMER5_ENABLE
    Handler Timer5Handlers[MAX_HANDLERS_PER_TIMER]; 
#endif
#ifdef TIMER6_ENABLE
    Handler Timer6Handlers[MAX_HANDLERS_PER_TIMER]; 
#endif
#ifdef TIMER7_ENABLE
    Handler Timer7Handlers[MAX_HANDLERS_PER_TIMER]; 
#endif
#ifdef TIMER8_ENABLE
    Handler Timer8Handlers[MAX_HANDLERS_PER_TIMER]; 
#endif

    void SchedulerInit();

    uint8_t AddHandler(void (*isr)(), double milliseconds);  
};

#ifdef TIMER0_ENABLE
void Timer0ISR();
#endif
#ifdef TIMER1_ENABLE
void Timer1ISR();
#endif
#ifdef TIMER2_ENABLE
void Timer2ISR();
#endif
#ifdef TIMER3_ENABLE
void Timer3ISR();
#endif
#ifdef TIMER4_ENABLE
void Timer4ISR();
#endif
#ifdef TIMER5_ENABLE
void Timer5ISR();
#endif
#ifdef TIMER6_ENABLE
void Timer6ISR();
#endif
#ifdef TIMER7_ENABLE
void Timer7ISR();
#endif
#ifdef TIMER8_ENABLE
void Timer8ISR();
#endif

uint8_t StartNewTimer(void (*isr)(), double milliseconds);

extern TimerScheduler Scheduler;
extern int16_t   TimerMs[MAX_TIMERS];
extern uint8_t   FreeTimers[MAX_TIMERS];
#endif