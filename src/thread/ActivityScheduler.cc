// belserich on 20.05.20

#include "thread/ActivityScheduler.h"

void ActivityScheduler::suspend()
{
	Activity* running = (Activity*) active();
	running->changeTo(Activity::BLOCKED);
	reschedule();
}

void ActivityScheduler::kill(Activity* activity)
{
	activity->changeTo(Activity::State::BLOCKED);
	readylist.remove(activity);

	Coroutine* co = activity;
	if (co == active())
	{
		reschedule();
	}
}

void ActivityScheduler::exit()
{
	kill((Activity*) active());
}

void ActivityScheduler::activate(Schedulable *to)
{
	Activity* oldRunning = (Activity*) active();
	if (!oldRunning->isBlocked() && !oldRunning->isZombie())
	{
		oldRunning->changeTo(Activity::READY);
		schedule(oldRunning);
	}

	Activity* newRunning = (Activity*) to;
	newRunning->changeTo(Activity::RUNNING);
	dispatch(newRunning);
}
