#include "thread/Scheduler.h"

// Einfuegen eines Elements in die Liste
void Scheduler::schedule(Schedulable *sched) {
	IntLock lock;
    readylist.enqueue((Chain*) sched);
}

// Entfernen des Elements von der Liste
void Scheduler::remove(Schedulable *sched) {
	IntLock lock;
    readylist.remove((Chain*) sched);
}

// Aktiviert das vorderste element der liste und holt es von d er liste
void Scheduler::reschedule() {
	IntLock lock;
    activate((Schedulable*) readylist.dequeue());
}