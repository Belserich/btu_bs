#ifndef Activity_h
#define Activity_h

/*
 * Activity:
 * Diese Klasse implementiert kooperative aktive Objekte
 * fuer Co-Stubs.
 *
 * Anmerkung:
 * Im Gegensatz zur Coroutine wird eine
 * Instanz der Klasse Activity von einem Scheduler verwaltet.
 * Ein explizites Umschalten zu einer anderen Coroutine
 * wird damit hinfaellig.
 */
#include <cstddef>
#include "thread/Schedulable.h"
#include "thread/Coroutine.h"


class Activity: public Schedulable, public Coroutine {
public:
	/* Die logischen Ausfuehrungszustaende
	 * fuer diese Aktivitaet
	 */
	enum State {
		BLOCKED,	//Prozesse die auf etwas warten
		READY,      	//Prozesse die aktiviert werden k�nnen
		RUNNING,	//Status des aktiven Prozesses
		ZOMBIE		//Kindprozesse die vor den Eltern beendet werden
	};

	/* Aufsetzen eines Threads, der initiale Zustand ist "Blocked",
	 * da der Thread erst laufen darf, wenn der spezielle Konstruktor
	 * der abgeleiteten Klasse abgearbeitet ist. Die Aktivierung
	 * erfolgt von der abgeleiteten Klasse mittels "wakeup".
	*/
	Activity(void* tos);

	/* Verpacken des aktuellen Kontrollflusses als Thread.
	 * Wird nur f�r den Hauptkontrollfluss "main" ben�tigt.
	 * Es wird hier kein Stack initialisiert.
	 * Beachte das Activity wegen der Vererbungsbeziehung von
	 * Coroutine abstrakt ist. Bei Bedarf muss "body" direkt
	 * aufgerufen werden.
	 */
	Activity();

	/* Im Destruktor muss ein explizites Terminieren dieser Aktivitaet erfolgen.
	 * Das muss geschehen, da aufgrund der Aufrufreihenfolge von
	 * Destruktoren bereits der abgeleitete Teil des Activity-Objekts zerstoert
	 * wurde und diese Aktivitaet daher keine Laufzeit mehr erhalten darf.
	 * Das Warten auf die Beendigung (mittels join()) muss im Destruktor der
	 * von Activity am weitesten abgeleiteten Klasse erfolgen.
	 */
	virtual ~Activity();
	static void operator delete(void* p){}

	/* Veranlasst den Scheduler, diese Aktivitaet zu suspendieren.
	 */
	void sleep();

	/* Veranlasst den Scheduler, diese Aktivitaet aufzuwecken.
	 */
	void wakeup();

	/* Diese Aktivitaet gibt die CPU vorruebergehend ab.
	 */
	void yield();

	/* Diese Aktivitaet wird terminiert. Hier muss eine eventuell
	 * auf die Beendigung wartende Aktivit�t geweckt werden.
	 */
	void exit();

	/* Der aktuelle Prozess wird solange schlafen gelegt, bis der
	 * Prozess auf dem join aufgerufen wird beendet ist. Das
	 * Wecken des wartenden Prozesses �bernimmt exit.
	 */
	void join();


	// Folgende Methoden d�rfen "inline" implementiert werden

	/* �ndern des Ausf�hrungszustandes. Diese Methode sollte nur vom
	 * Scheduler verwendet werden.
	 */
	void changeTo(State state)
	{
		this->state = state;
	}

	// Ausf�hrungszustand abfragen.
	bool isBlocked()
	{
		return (this->state == BLOCKED)?(true):(false);
	}

	bool isReady()
	{
		return (this->state == READY)?(true):(false);
	}

	bool isRunning()
	{
		return (this->state == RUNNING)?(true):(false);
	}

	bool isZombie()
	{
		return (this->state == ZOMBIE)?(true):(false);
	}
	
	//Gibt den State als state zur�ck
	State getState() {
		return this->state;		
	}
	
	/* getSuccessor Methode
	 * gibt den Successor raus
	 */
    Activity* getSuccessor() {
		return this->successor;
	}
	
	/* setSuccessor Methode
	 * setzt den Successor
	 */
	void setSuccessor(Activity* act) {
		this->successor = act;
	}


private:
	State state;
	Activity* successor = NULL;
};

#endif
