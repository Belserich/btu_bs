// belserich on 20.05.20

#include <interrupts/IntLock.h>
#include "thread/Scheduler.h"

void Scheduler::schedule(Schedulable *sched)
{
	IntLock lock;
	readylist.enqueue(sched);
}

void Scheduler::remove(Schedulable *sched)
{
	IntLock lock;
	readylist.remove(sched);
}

void Scheduler::reschedule()
{
	IntLock lock;
	activate((Schedulable*) readylist.dequeue());
}
