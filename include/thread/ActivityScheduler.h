#ifndef ActivityScheduler_h
#define ActivityScheduler_h

/*
 * ActivityScheduler: 	Diese Klasse implementiert die Zustandsverwaltung
 * 			für Activities
 *
 */
#include <cstddef>
#include "lib/Queue.h"
#include "thread/Activity.h"
#include "thread/Dispatcher.h"
#include "thread/Scheduler.h"
#include "interrupts/IntLock.h"



class ActivityScheduler: public Dispatcher, public Scheduler {
public:
	ActivityScheduler()
	{
	}

	/* Initialisieren der ersten Aktivität, des Schedulers
	 * und des Dispatchers.
	 * Wird nur einmal aufgerufen.
	 */
	void start(Activity* act)
	{
		init(act);
		act->changeTo(Activity::RUNNING);
	}

	/* Suspendieren des aktiven Prozesses
	 * Der korrekte Ausfuehrungszustand ist zu setzen
	 * und danach der naechste lauffaehige Prozess zu aktivieren.
	 */
	void suspend();

	/* Explizites Terminieren des angegebenen Prozesses
	 * Der Prozesszustand ist korrekt zu setzen
	 * und der Prozess aus der Ready-Liste des Schedulers
	 * zu entfernen. Falls der aktive Prozess zerstoert wird,
	 * ist dem naechsten lauffaehigen Prozess die CPU
	 * zuzuteilen.
	 */
	void kill(Activity* act);

	/* Terminieren des aktiven Prozesses,
	 * und Wechsel zum naechsten lauffaehigen Prozess
	 */
	void exit();
	/*	getActiveActivity Methode
	*	gibt den activen Prozess wieder
	*/
	Activity* getActiveActivity() {
		return ((Activity*)active());
	};
	
	void checkSlice();
	
protected:
	/* Der aktive Prozess ist, sofern er sich nicht im Zustand
	 * Blocked oder Zombie befindet, wieder auf die Ready-Liste
	 * zu setzen. Danach ist "to" mittels dispatch die Kontrolle
	 * zu übergeben.
	 */
	virtual void activate(Schedulable* to);

private:
	bool idle = false;;
};

extern ActivityScheduler scheduler;

#endif
