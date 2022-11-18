#include "scheduler.h"


TimerScheduler Scheduler;

int16_t  TimerMs[MAX_TIMERS]
{
#ifdef TIMER0_ENABLE
    TIMER0_MS,
#else
    -1,
#endif
#ifdef TIMER1_ENABLE
    TIMER1_MS,
#else
    -1,
#endif
#ifdef TIMER2_ENABLE
    TIMER2_MS,
#else
    -1,
#endif
#ifdef TIMER3_ENABLE
    TIMER3_MS,
#else
    -1,
#endif
#ifdef TIMER4_ENABLE
    TIMER4_MS,
#else
    -1,
#endif
#ifdef TIMER5_ENABLE
    TIMER5_MS,
#else
    -1,
#endif
#ifdef TIMER6_ENABLE
    TIMER6_MS,
#else
    -1,
#endif
#ifdef TIMER7_ENABLE
    TIMER7_MS,
#else
    -1,
#endif
#ifdef TIMER8_ENABLE
    TIMER8_MS,
#else
    -1,
#endif
};

uint8_t  FreeTimers[MAX_TIMERS]
{
#if defined (TIMER0_ENABLE) || defined(TIMER0_RESERVED)
    0,
#else
    1,
#endif
#if defined (TIMER1_ENABLE) || defined(TIMER1_RESERVED)
    0,
#else
    1,
#endif
#if defined (TIMER2_ENABLE) || defined(TIMER2_RESERVED)
    0,
#else
    1,
#endif
#if defined (TIMER3_ENABLE) || defined(TIMER3_RESERVED)
    0,
#else
    1,
#endif
#if defined (TIMER4_ENABLE) || defined(TIMER4_RESERVED)
    0,
#else
    1,
#endif
#if defined (TIMER5_ENABLE) || defined(TIMER5_RESERVED)
    0,
#else
    1,
#endif
#if defined (TIMER6_ENABLE) || defined(TIMER6_RESERVED)
    0,
#else
    1,
#endif
#if defined (TIMER7_ENABLE) || defined(TIMER7_RESERVED)
    0,
#else
    1,
#endif
#if defined (TIMER8_ENABLE) || defined(TIMER8_RESERVED)
    0,
#else
    1,
#endif
};

void Handler::Run()
{
    timerTicks++;
    if (timerTicks >= waitTimerTicks)
    {
        isr();
        timerTicks=0;
    }
}

void Handler::Start(void (*setISR)(), uint16_t setWaitTimerTicks, uint8_t setTimerIndex)
{
    isr = setISR;
    waitTimerTicks = setWaitTimerTicks;
    timerIndex = setTimerIndex;
    hasISR = 1;
}

void TimerScheduler::SchedulerInit() 
{
#ifdef TIMER0_ENABLE
    Timer0.attachInterrupt(Timer0ISR).start(TIMER0_MS*1000);
#endif
#ifdef TIMER1_ENABLE
    Timer1.attachInterrupt(Timer1ISR).start(TIMER1_MS*1000);
#endif
#ifdef TIMER2_ENABLE
    Timer2.attachInterrupt(Timer2ISR).start(TIMER2_MS*1000);
#endif
#ifdef TIMER3_ENABLE
    Timer3.attachInterrupt(Timer3ISR).start(TIMER3_MS*1000);
#endif
#ifdef TIMER4_ENABLE
    Timer4.attachInterrupt(Timer4ISR).start(TIMER4_MS*1000);
#endif
#ifdef TIMER5_ENABLE
    Timer5.attachInterrupt(Timer5ISR).start(TIMER5_MS*1000);
#endif
#ifdef TIMER6_ENABLE
    Timer6.attachInterrupt(Timer6ISR).start(TIMER6_MS*1000);
#endif
#ifdef TIMER7_ENABLE
    Timer7.attachInterrupt(Timer7ISR).start(TIMER7_MS*1000);
#endif
#ifdef TIMER8_ENABLE
    Timer8.attachInterrupt(Timer8ISR).start(TIMER8_MS*1000);
#endif
}

uint8_t TimerScheduler::AddHandler(void (*isr)(), double milliseconds)
{
    uint8_t i;
    uint8_t lowestTicks = 255;
    uint8_t lowestDelta = 255;
    uint8_t numTicks;
    uint8_t delta;
    uint8_t timerIndex;
    uint8_t setISR = 0;

    for (i = 0; i < MAX_TIMERS; i++)
    {
        if (TimerMs[i] > 0)
        {
            numTicks = (uint8_t)floor((float)milliseconds/(float)TimerMs[i]);
            if (numTicks != 0 && (double)TimerMs[i]*(double)numTicks != (double)milliseconds)
            {
                numTicks++;
            }
            if (numTicks != 0 && numTicks < lowestTicks)
            {
                lowestTicks = numTicks;
                timerIndex = i;
            }
        }
    }

    if (lowestTicks == 255)
    {
        for (i = 0; i < MAX_TIMERS; i++)
        {
            if (TimerMs[i] > 0)
            {
                delta = (uint8_t)milliseconds - (uint8_t)TimerMs[i];
                if (delta < lowestDelta)
                {
                    lowestDelta = delta;
                    lowestTicks = 1;
                    timerIndex = i;
                }
            }
        }
    }

    if (lowestTicks == 255)
    {
        return StartNewTimer(isr, milliseconds);
    }

    switch (timerIndex)
    {
#ifdef TIMER0_ENABLE
    case 0:
        for (i = 0; i < MAX_HANDLERS_PER_TIMER; i++)
        {
            if(!Timer0Handlers[i].hasISR)
            {
                Timer0Handlers[i].Start(isr, lowestTicks, timerIndex);
                setISR=1;
            }
        }
        break;
#endif
#ifdef TIMER1_ENABLE
    case 1:
        for (i = 0; i < MAX_HANDLERS_PER_TIMER; i++)
        {
            if(!Timer1Handlers[i].hasISR)
            {
                Timer1Handlers[i].Start(isr, lowestTicks, timerIndex);
                setISR=1;
            }
        }
        break;
#endif
#ifdef TIMER2_ENABLE
    case 2:
        for (i = 0; i < MAX_HANDLERS_PER_TIMER; i++)
        {
            if(!Timer2Handlers[i].hasISR)
            {
                Timer2Handlers[i].Start(isr, lowestTicks, timerIndex);
                setISR=1;
            }
        }
        break;
#endif
#ifdef TIMER3_ENABLE
    case 3:
        for (i = 0; i < MAX_HANDLERS_PER_TIMER; i++)
        {
            if(!Timer3Handlers[i].hasISR)
            {
                Timer3Handlers[i].Start(isr, lowestTicks, timerIndex);
                setISR=1;
            }
        }
        break;
#endif
#ifdef TIMER4_ENABLE
    case 4:
        for (i = 0; i < MAX_HANDLERS_PER_TIMER; i++)
        {
            if(!Timer4Handlers[i].hasISR)
            {
                Timer4Handlers[i].Start(isr, lowestTicks, timerIndex);
                setISR=1;
            }
        }
        break;
#endif
#ifdef TIMER5_ENABLE
    case 5:
        for (i = 0; i < MAX_HANDLERS_PER_TIMER; i++)
        {
            if(!Timer5Handlers[i].hasISR)
            {
                Timer5Handlers[i].Start(isr, lowestTicks, timerIndex);
                setISR=1;
            }
        }
        break;
#endif
#ifdef TIMER6_ENABLE
    case 6:
        for (i = 0; i < MAX_HANDLERS_PER_TIMER; i++)
        {
            if(!Timer6Handlers[i].hasISR)
            {
                Timer6Handlers[i].Start(isr, lowestTicks, timerIndex);
                setISR=1;
            }
        }
        break;
#endif
#ifdef TIMER7_ENABLE
    case 7:
        for (i = 0; i < MAX_HANDLERS_PER_TIMER; i++)
        {
            if(!Timer7Handlers[i].hasISR)
            {
                Timer7Handlers[i].Start(isr, lowestTicks, timerIndex);
                setISR=1;
            }
        }
        break;
#endif
#ifdef TIMER8_ENABLE
    case 8:
        for (i = 0; i < MAX_HANDLERS_PER_TIMER; i++)
        {
            if(!Timer8Handlers[i].hasISR)
            {
                Timer8Handlers[i].Start(isr, lowestTicks, timerIndex);
                setISR=1;
            }
        }
        break;
#endif
    default:
        break;
    }

    if (setISR)
    {
        return NO_ERROR;
    }
    else
    {
        return StartNewTimer(isr, milliseconds);
    }
}


uint8_t StartNewTimer(void (*isr)(), double milliseconds)
{
    uint8_t i;
    uint8_t setISR = 0;
    for (i=0; i<MAX_TIMERS;i++)
    {
        if (FreeTimers[i])
        {
            switch(i)
            {
#ifndef TIMER0_RESERVED
                case 0:
                    Timer0.attachInterrupt(isr).start(milliseconds*1000);
                    FreeTimers[i] = 0;
                    setISR = 1;
                    break;
#endif
#ifndef TIMER1_RESERVED
                case 1:
                    Timer1.attachInterrupt(isr).start(milliseconds*1000);
                    FreeTimers[i] = 0;
                    setISR = 1;
                    break;
#endif
#ifndef TIMER2_RESERVED
                case 2:
                    Timer2.attachInterrupt(isr).start(milliseconds*1000);
                    FreeTimers[i] = 0;
                    setISR = 1;
                    break;
#endif
#ifndef TIMER3_RESERVED
                case 3:
                    Timer3.attachInterrupt(isr).start(milliseconds*1000);
                    FreeTimers[i] = 0;
                    setISR = 1;
                    break;
#endif
#ifndef TIMER4_RESERVED
                case 4:
                    Timer4.attachInterrupt(isr).start(milliseconds*1000);
                    FreeTimers[i] = 0;
                    setISR = 1;
                    break;
#endif
#ifndef TIMER5_RESERVED
                case 5:
                    Timer5.attachInterrupt(isr).start(milliseconds*1000);
                    FreeTimers[i] = 0;
                    setISR = 1;
                    break;
#endif
#ifndef TIMER6_RESERVED
                case 6:
                    Timer6.attachInterrupt(isr).start(milliseconds*1000);
                    FreeTimers[i] = 0;
                    setISR = 1;
                    break;
#endif
#ifndef TIMER7_RESERVED
                case 7:
                    Timer7.attachInterrupt(isr).start(milliseconds*1000);
                    FreeTimers[i] = 0;
                    setISR = 1;
                    break;
#endif
#ifndef TIMER8_RESERVED
                case 8:
                    Timer8.attachInterrupt(isr).start(milliseconds*1000);
                    FreeTimers[i] = 0;
                    setISR = 1;
                    break;
#endif
                default:
                    break;
            }
        }
    }
    if (setISR)
    {
        return NO_ERROR;
    }
    else
    {
        return NO_TIMERS_ERROR;
    }
}

#ifdef TIMER0_ENABLE
void Timer0ISR(){
    uint8_t i;
    for (i = 0; i < MAX_HANDLERS_PER_TIMER; i++)
    {
        if(Scheduler.Timer0Handlers[i].hasISR)
        {
            Scheduler.Timer0Handlers[i].Run();
        }
    }
}
#endif
#ifdef TIMER1_ENABLE
void Timer1ISR(){
    uint8_t i;
    for (i = 0; i < MAX_HANDLERS_PER_TIMER; i++)
    {
        if(Scheduler.Timer1Handlers[i].hasISR)
        {
            Scheduler.Timer1Handlers[i].Run();
        }
    }
}
#endif
#ifdef TIMER2_ENABLE
void Timer2ISR(){
    uint8_t i;
    for (i = 0; i < MAX_HANDLERS_PER_TIMER; i++)
    {
        if(Scheduler.Timer2Handlers[i].hasISR)
        {
            Scheduler.Timer2Handlers[i].Run();
        }
    }
}
#endif
#ifdef TIMER3_ENABLE
void Timer3ISR(){
    uint8_t i;
    for (i = 0; i < MAX_HANDLERS_PER_TIMER; i++)
    {
        if(Scheduler.Timer3Handlers[i].hasISR)
        {
            Scheduler.Timer3Handlers[i].Run();
        }
    }
}
#endif
#ifdef TIMER4_ENABLE
void Timer4ISR(){
    uint8_t i;
    for (i = 0; i < MAX_HANDLERS_PER_TIMER; i++)
    {
        if(Scheduler.Timer4Handlers[i].hasISR)
        {
            Scheduler.Timer4Handlers[i].Run();
        }
    }
}
#endif
#ifdef TIMER5_ENABLE
void Timer5ISR(){
    uint8_t i;
    for (i = 0; i < MAX_HANDLERS_PER_TIMER; i++)
    {
        if(Scheduler.Timer5Handlers[i].hasISR)
        {
            Scheduler.Timer5Handlers[i].Run();
        }
    }
}
#endif
#ifdef TIMER6_ENABLE
void Timer6ISR(){
    uint8_t i;
    for (i = 0; i < MAX_HANDLERS_PER_TIMER; i++)
    {
        if(Scheduler.Timer6Handlers[i].hasISR)
        {
            Scheduler.Timer6Handlers[i].Run();
        }
    }
}
#endif
#ifdef TIMER7_ENABLE
void Timer7ISR(){
    uint8_t i;
    for (i = 0; i < MAX_HANDLERS_PER_TIMER; i++)
    {
        if(Scheduler.Timer7Handlers[i].hasISR)
        {
            Scheduler.Timer7Handlers[i].Run();
        }
    }
}
#endif
#ifdef TIMER8_ENABLE
void Timer8ISR(){
    uint8_t i;
    for (i = 0; i < MAX_HANDLERS_PER_TIMER; i++)
    {
        if(Scheduler.Timer8Handlers[i].hasISR)
        {
            Scheduler.Timer8Handlers[i].Run();
        }
    }
}
#endif