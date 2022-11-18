/*
  DueTimer.h - DueTimer header file, definition of methods and attributes...
  For instructions, go to https://github.com/ivanseidel/DueTimer

  Created by Ivan Seidel Gomes, March, 2013.
  Modified by Philipp Klaus, June 2013.
  Released into the public domain.
*/

#include "Arduino.h"

#if defined(_SAM3XA_)

#ifndef DueTimer_h
#define DueTimer_h

#include <inttypes.h>
#include "scheduler.h"

/*
	This fixes compatibility for Arduono Servo Library.
	Uncomment to make it compatible.

	Note that:
		+ Timers: 0,2,3,4,5 WILL NOT WORK, and will
				  neither be accessible by Timer0,...
*/
// #define USING_SERVO_LIB	true

#ifdef USING_SERVO_LIB
	#warning "HEY! You have set flag USING_SERVO_LIB. Timer0, 2,3,4 and 5 are not available"
#endif


#if defined TC2
#define NUM_TIMERS  9
#else
#define NUM_TIMERS  6
#endif

class DueTimer
{
protected:

	// Represents the timer id (index for the array of Timer structs)
	const unsigned short timer;

	// Stores the object timer frequency
	// (allows to access current timer period and frequency):
	static double _frequency[NUM_TIMERS];

	// Picks the best clock to lower the error
	static uint8_t bestClock(double frequency, uint32_t& retRC);

  // Make Interrupt handlers friends, so they can use callbacks
#ifndef TIMER0_RESERVED
  friend void TC0_Handler(void);
#endif
#ifndef TIMER1_RESERVED
  friend void TC1_Handler(void);
#endif
#ifndef TIMER2_RESERVED
  friend void TC2_Handler(void);
#endif
#ifndef TIMER3_RESERVED
  friend void TC3_Handler(void);
#endif
#ifndef TIMER4_RESERVED
  friend void TC4_Handler(void);
#endif
#ifndef TIMER5_RESERVED
  friend void TC5_Handler(void);
#endif
#if NUM_TIMERS > 6
#ifndef TIMER6_RESERVED
  friend void TC6_Handler(void);
#endif
#ifndef TIMER7_RESERVED
  friend void TC7_Handler(void);
#endif
#ifndef TIMER0_RESERVED
  friend void TC8_Handler(void);
#endif
#endif

	

	struct Timer
	{
		Tc *tc;
		uint32_t channel;
		IRQn_Type irq;
	};

	// Store timer configuration (static, as it's fixed for every object)
	static const Timer Timers[NUM_TIMERS];

public:
	static void (*callbacks[NUM_TIMERS])();

	static DueTimer getAvailable(void);

	DueTimer(unsigned short _timer);
	DueTimer& attachInterrupt(void (*isr)());
	DueTimer& detachInterrupt(void);
	DueTimer& start(double microseconds = -1);
	DueTimer& stop(void);
	DueTimer& setFrequency(double frequency);
	DueTimer& setPeriod(double microseconds);

	double getFrequency(void) const;
	double getPeriod(void) const;

  inline __attribute__((always_inline)) bool operator== (const DueTimer& rhs) const
    {return timer == rhs.timer; };
  inline __attribute__((always_inline)) bool operator!= (const DueTimer& rhs) const
    {return timer != rhs.timer; };
};

// Just to call Timer.getAvailable instead of Timer::getAvailable() :
extern DueTimer Timer;

#ifndef TIMER1_RESERVED
extern DueTimer Timer1;
#endif
// Fix for compatibility with Servo library
#ifndef USING_SERVO_LIB
#ifndef TIMER0_RESERVED
	extern DueTimer Timer0;
#endif
#ifndef TIMER2_RESERVED
	extern DueTimer Timer2;
#endif
#ifndef TIMER3_RESERVED
	extern DueTimer Timer3;
#endif
#ifndef TIMER4_RESERVED
	extern DueTimer Timer4;
#endif
#ifndef TIMER5_RESERVED
	extern DueTimer Timer5;
#endif
#endif
#if NUM_TIMERS > 6
#ifndef TIMER6_RESERVED
extern DueTimer Timer6;
#endif
#ifndef TIMER7_RESERVED
extern DueTimer Timer7;
#endif
#ifndef TIMER8_RESERVED
extern DueTimer Timer8;
#endif
#endif

#endif

#else
	#error Oops! Trying to include DueTimer on another device?
#endif
