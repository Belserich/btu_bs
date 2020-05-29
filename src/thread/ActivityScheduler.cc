// belserich on 20.05.20

#include "thread/ActivityScheduler.h"

extern PrintStream out;

/* Suspendieren des aktiven Prozesses
 * Der korrekte Ausfuehrungszustand ist zu setzen
 * und danach der naechste lauffaehige Prozess zu aktivieren.
 */
void ActivityScheduler::suspend()
{
	Activity* running = (Activity*) active();
	running->changeTo(Activity::BLOCKED);
	reschedule();
}

/* Explizites Terminieren des angegebenen Prozesses
 * Der Prozesszustand ist korrekt zu setzen
 * und der Prozess aus der Ready-Liste des Schedulers
 * zu entfernen. Falls der aktive Prozess zerstoert wird,
 * ist dem naechsten lauffaehigen Prozess die CPU
 * zuzuteilen.
 */
void ActivityScheduler::kill(Activity* activity)
{
//	out.print("Toete Aktivitaet ");
//	out.print(activity->name());

	activity->changeTo(Activity::State::ZOMBIE);

	Coroutine* co = activity;
	if (co == active()) // ist die zu toetende Coroutine gerade am Laufen?
	{
//		out.println(" die gerade lief.");
		reschedule();
	}
	else
	{
		readylist.remove(activity);
	}
}

/* Terminieren des aktiven Prozesses,
 * und Wechsel zum naechsten lauffaehigen Prozess
 */
void ActivityScheduler::exit()
{
	Activity* running = (Activity*) active();
	running->changeTo(Activity::ZOMBIE);
	reschedule();
}

void ActivityScheduler::activate(Schedulable *to)
{
	Activity* oldRunning = (Activity*) active();
//	out.print("Lief gerade: ");
//	out.print(oldRunning->name());

	if (!oldRunning->isBlocked() && !oldRunning->isZombie())
	{
//		out.print(" und wird neu eingereiht.");
		oldRunning->wakeup();
	}

	if (to == nullptr)
	{
//		out.print("und wird neu fortgesetzt");
		to = (Activity*) readylist.dequeue();
	}

	if (to != nullptr)
	{
		Activity* newRunning = (Activity*) to;
//		out.print(" Laeuft jetzt: ");
//		out.print(newRunning->name());

		newRunning->changeTo(Activity::RUNNING);
		dispatch(newRunning); // wechsle Coroutine
	}

//	out.println();
}
