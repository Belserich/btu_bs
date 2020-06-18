#include "thread/Activity.h"
#include "thread/ActivityScheduler.h"

/*	Konstruuktoren setzten initial Zustand
	und starten ggf den Prozess
	
*/
Activity::Activity(void *tos) : Coroutine(tos) {
	IntLock lock;
    this->state = BLOCKED;
};

Activity::Activity() {
    this->state = BLOCKED;
    scheduler.start(this);
};

/*
	Destruktor
	killt den Prozess
*/
Activity::~Activity() {
    scheduler.kill(this);
};

/* Sleep Methode
	Legt den Prozessor schlafen und stellt ihn hinten wieder in die Liste
*/
void Activity::sleep() {
	scheduler.suspend();
};

/*	Wakeup Methode
	Weckt den Prozess auf und scheduelt ihn
*/
void Activity::wakeup() {
	IntLock lock;
	this->state = READY;
	scheduler.schedule(this);
};

/* yield Methode
	Prozess wird pausiert und neuer wird aktiviert
*/
void Activity::yield() {
    scheduler.reschedule(); 
};

/* exit Methode
	schließt den Prozess und setzt ihn als Zombie
*/
void Activity::exit() {
    this->state = ZOMBIE;
    scheduler.exit();
};

/* join Metehode
	setzt den activen Prozess als Erbe des neuen
	und suspendet den  activen  Prozess und holt ein neuen
*/
void Activity::join() {
	IntLock lock;
	if(this != scheduler.getActiveActivity()) {
		if ((!isZombie())) { 
			setSuccessor(scheduler.getActiveActivity());
			scheduler.suspend();
		}
    }
};