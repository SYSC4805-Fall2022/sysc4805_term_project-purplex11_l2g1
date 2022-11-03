#include <stdlib.h>
#include "DueTimerCWrapper.h"
#include "DueTimer.h"

struct DueTimer {
    void *obj;
};

DueTimer_t *NewTimer(int index)
{
    DueTimer_t *timer;
    DueTimer   *obj;

    timer = (typeof(timer))malloc(sizeof(*timer));

    switch(index)
    {
        case 0:
            obj = Timer0;
            break;
        case 1:
            obj = Timer1;
            break;
        case 2:
            obj = Timer2;
            break;
        case 3:
            obj = Timer3;
            break;
        case 4:
            obj = Timer4;
            break;
        case 5:
            obj = Timer5;
            break;
        case 6:
            obj = Timer6;
            break;
        case 7:
            obj = Timer7;
            break;
        case 8:
            obj = Timer8;
            break;
    }
    timer->obj = obj;

    return timer;
}

DueTimer_t * TimerAttachInterrupt(DueTimer_t * timer, void * isr)
{
    DueTimer *obj;

    if (timer == NULL)
        return timer;
    
    obj = static_cast<DueTimer *>(timer->obj);

    return obj.attachInterrupt(isr);
}


DueTimer_t * TimerDetachInterrupt(DueTimer_t * timer)
{
    DueTimer *obj;

    if (timer == NULL)
        return timer;
    
    obj = static_cast<DueTimer *>(timer->obj);

    return obj.detatchInterrupt();
}

DueTimer_t * TimerStart(DueTimer_t * timer, long microseconds)
{
    DueTimer *obj;

    if (timer == NULL)
        return timer;
    
    obj = static_cast<DueTimer *>(timer->obj);

    return obj.start(microseconds);
}

DueTimer_t * TimerStop(DueTimer_t * timer)
{
    DueTimer *obj;

    if (timer == NULL)
        return timer;
    
    obj = static_cast<DueTimer *>(timer->obj);

    return obj.stop();
}

DueTimer_t * TimerSetFrequency(DueTimer_t * timer, double frequency)
{
    DueTimer *obj;

    if (timer == NULL)
        return timer;
    
    obj = static_cast<DueTimer *>(timer->obj);

    return obj.setFrequency(frequency);
}

DueTimer_t * TimerSetPeriod(DueTimer_t * timer, unsigned long microseconds)
{
    DueTimer *obj;

    if (timer == NULL)
        return timer;
    
    obj = static_cast<DueTimer *>(timer->obj);

    return obj.setPeriod(microseconds);
}

double TimerGetFrequency(DueTimer_t * timer)
{
    DueTimer *obj;

    if (timer == NULL)
        return timer;
    
    obj = static_cast<DueTimer *>(timer->obj);

    return obj.getFrequency();
}

long TimerGetPeriod(DueTimer_t * timer)
{
    DueTimer *obj;

    if (timer == NULL)
        return timer;
    
    obj = static_cast<DueTimer *>(timer->obj);

    return obj.getPeriod();
}
