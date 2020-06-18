#include "thread/ActivityScheduler.h"
#include "io/PrintStream.h"

extern PrintStream out;

/*
	suspend Methode
	blockiert den Prozess und setzt ihn wieder in die Liste
*/
void ActivityScheduler::suspend() {
	IntLock lock;
	getActiveActivity()->changeTo(Activity::BLOCKED);
    reschedule();
};

/*
	kill Methode
	Macht den Prozess zum Zombie
	und je nach dem ob er läuft holt er ihn
	von der CPU runter oder setzt ihn wieder in die Liste

*/
void ActivityScheduler::kill(Activity *act) {
	IntLock lock;
    act->changeTo(Activity::ZOMBIE);
    if (act != getActiveActivity()) {
        remove((Schedulable *) act);
    } else {
        reschedule();
    }
};

/*
	exit Methode
	schaut ob es noch Erben gibt und weckt diese 
	und setzt dann den Prozess wieder in die Liste

*/
void ActivityScheduler::exit() {
	IntLock lock;
	Activity* act = getActiveActivity();
    if (act->getSuccessor() != NULL) {
        act->getSuccessor()->wakeup();
        act->setSuccessor(NULL);
    }
    reschedule();
};

/*
	activate Methode
	Holt solange Prozesse aus der Liste bis einer da ist,
	wenn der active prozess noch weiterleben darf wird er wieder gescheduled
	und fliegender wechsel der Prozesse wird gemacht

*/
void ActivityScheduler::activate(Schedulable *to) {
	Activity* act = getActiveActivity();
	Activity* nTo = (Activity*)to;
	if(act != nTo) {
		if (nTo != 0) {
			if (!(act->isZombie()) && !(act->isBlocked())) {
				act->changeTo(Activity::READY);
				//act->setTime(0);
				schedule(act);
			}
			//Wechsel
			nTo->changeTo(Activity::RUNNING);
			//(Activity*)this -> active()
			act = nTo;
			dispatch(nTo);
		} else {
			if((act->isBlocked()) || (act->isZombie())) {
				idle = true;
				while (!nTo) { 
					CPU::enableInterrupts();
					CPU::halt();
					CPU::disableInterrupts();
					nTo = (Activity*) readylist.dequeue();
				} 
				idle = false;
				nTo->changeTo(Activity::RUNNING);
				dispatch(nTo);
			}
		}	
	}
};

/* 
	checkSlice Methode
	überprüft ob die vergangene Zeit großer ist als die zugeordnete Zeit und führt dann einen Wechsel durch
	oder erhöht die Zeit

*/
void ActivityScheduler::checkSlice() {
	if(!idle) {
		IntLock lock;
		Activity* act = getActiveActivity();
		if (act->timeOver()) {
			//out.println("Time over");
			act->setTime(0);
			reschedule();
		} else {
			act->incTime();
		}
	}
};