// belserich on 13.05.20

#include "thread/Activity.h"
#include "thread/ActivityScheduler.h"

Activity::Activity(void *tos)
	: Coroutine(tos)
{}

Activity::Activity()
	: Coroutine()
{
	scheduler.start(this);
}

Activity::~Activity()
{
	scheduler.kill(this);
}

void Activity::sleep()
{
	scheduler.suspend();
}

void Activity::wakeup()
{
	changeTo(READY);
	scheduler.schedule(this);
}

void Activity::yield()
{
	scheduler.reschedule();
}

void Activity::exit()
{

}

void Activity::join()
{
	changeTo(ZOMBIE);
	scheduler.dispatch(this);
}

