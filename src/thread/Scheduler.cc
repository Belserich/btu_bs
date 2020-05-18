#include "thread/Scheduler.h"

// Einfuegen eines neuen Elements in die Ready-Liste.
void Scheduler::schedule(Schedulable* sched) {
    this -> readylist.enqueue(sched);           //Element hinten an die Queue angehängt
};

// Entfernen eines Elements von der Ready-Liste.
void Scheduler::remove(Schedulable* sched) {
    this -> readylist.remove(sched);            // bestimmtes Element aus Queue entfernen
};

// Aktiviert das vorderste der Liste mittels activate.
void Scheduler::reschedule() {
    this -> activate((Schedulable*) readylist.dequeue()); // vorderstes Element ist readylist.dequeue
};
