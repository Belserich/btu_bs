#include "thread/ActivityScheduler.h"


/* Suspendieren des aktiven Prozesses
 * Der korrekte Ausfuehrungszustand ist zu setzen
 * und danach der naechste lauffaehige Prozess zu aktivieren.
 */
void ActivityScheduler::suspend() {
    Activity* process = (Activity *) (this -> active());  //mit active bekommt man den Zeiger auf den aktiven Prozess
    process -> changeTo(Activity::BLOCKED);
    scheduler.reschedule();
};

/* Explizites Terminieren des angegebenen Prozesses
 * Der Prozesszustand ist korrekt zu setzen
 * und der Prozess aus der Ready-Liste des Schedulers
 * zu entfernen. Falls der aktive Prozess zerstoert wird,
 * ist dem naechsten lauffaehigen Prozess die CPU
 * zuzuteilen.
 */
void ActivityScheduler::kill(Activity* activity) {
        activity -> changeTo(Activity::ZOMBIE);
        scheduler.remove(activity);

        Activity* activeActivity =(Activity*) this -> active();
        if (activeActivity == activity) { //Falls der zu terminierende Prozess auch der aktive ist..
            scheduler.remove(activeActivity);
            scheduler.reschedule();  //..soll der nächste in der Liste aktiviert werden
        }
        else {
            scheduler.remove(activity);
        }

}

/* Terminieren des aktiven Prozesses,
 * und Wechsel zum naechsten lauffaehigen Prozess
 */
void ActivityScheduler::exit() {
    Activity* activity =(Activity*) this -> active(); //aktuell aktiver Prozess..
    scheduler.kill(activity);   //..wird terminiert..
    scheduler.reschedule();     //..und Wechsel zum nächsten Prozess
}

/* Der aktive Prozess ist, sofern er sich nicht im Zustand
	 * Blocked oder Zombie befindet, wieder auf die Ready-Liste
	 * zu setzen. Danach ist "to" mittels dispatch die Kontrolle
	 * zu �bergeben.
	 */

void ActivityScheduler::activate(Schedulable* to) {
    Activity* now = (Activity*) this -> active();  //aktiver Prozess
    Activity* next = (Activity*) to;
    if ((now -> isReady()) || (now -> isRunning()))  //einer der beiden anderen Zustände
        scheduler.schedule(now);                     //Prozess wieder an Queue anhängen

    next -> changeTo(Activity::RUNNING);        //Kontrolle an "to" übergeben
    dispatch(next);

}