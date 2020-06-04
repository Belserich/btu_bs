// belserich on 02.06.20

#include <device/PIC.h>
#include <interrupts/InterruptVector.h>
#include <io/PrintStream.h>
#include <thread/ActivityScheduler.h>
#include "device/Clock.h"

extern PIC pic;
extern PrintStream out;
extern ActivityScheduler scheduler;

Clock::Clock()
	: Gate(Timer), PIT(), m_Ticks(0)
{}

Clock::Clock(int us)
	: Gate(Timer), PIT(us), m_Ticks(0)
{
	pic.enable(0);
}

Clock::~Clock()
{
	pic.disable(0);
}

void Clock::windup(int us)
{
	static bool debug = false;
	if (us >= 0)
	{
		interval(us);
		pic.enable(0);
	}
	else
	{
		if (debug)
		{
			out.print("Disabling clock.");
		}
		pic.disable(0);
	}
}

void Clock::handle()
{
	static bool test = false;

	pic.ack();
	m_Ticks += 1;

	if (test)
	{
		int clockPhase = (m_Ticks / 50) % 4;
		char ch;
		switch (clockPhase)
		{
			case 0: ch = '-'; break;
			case 1: ch = '\\'; break;
			case 2: ch = '|'; break;
			case 3: ch = '/'; break;
			default: ch = '?'; break;
		}
		out.print(ch);
		out.print("\r");
	}
	else
	{
		scheduler.checkSlice();
	}
}
