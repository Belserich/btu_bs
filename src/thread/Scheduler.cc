// belserich on 20.05.20

#include "thread/Scheduler.h"

void Scheduler::schedule(Schedulable *sched)
{
	readylist.enqueue(sched);
}

void Scheduler::remove(Schedulable *sched)
{
	readylist.remove(sched);
}

void Scheduler::reschedule()
{
	activate((Schedulable*) readylist.dequeue());
}
