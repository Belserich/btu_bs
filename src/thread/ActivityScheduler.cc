// belserich on 20.05.20

#include <device/Clock.h>
#include "thread/ActivityScheduler.h"

extern PrintStream out;
extern Clock clock;

ActivityScheduler::~ActivityScheduler()
{
	clock.windup(-1);
}

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
	static bool debug = false;

	if (debug)
	{
		out.print("Toete Aktivitaet ");
		out.print(activity->name());
		if (activity == active())
		{
			out.println(" die gerade lief.");
		}
		else
		{
			out.println();
		}
	}

	activity->changeTo(Activity::State::ZOMBIE);
	if (activity == active()) // ist die zu toetende Coroutine gerade am Laufen?
	{
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
	static bool debug = false;

	Activity* oldRunning = (Activity*) active();

	if (debug)
	{
		out.print("Lief gerade: ");
		out.print(oldRunning->name());
		if (!oldRunning->isBlocked() && !oldRunning->isZombie())
		{
			out.print(" (neu eingereiht)");
		}
	}

	if (!oldRunning->isBlocked() && !oldRunning->isZombie())
	{
		oldRunning->wakeup();
	}

	if (to == nullptr)
	{
		to = (Activity*) readylist.dequeue();
		if (to != nullptr && debug)
		{
			out.print("(fortgesetzt)");
		}
	}

	if (to != nullptr)
	{
		Activity* newRunning = (Activity*) to;

		if (debug)
		{
			out.print("; Laeuft jetzt: ");
			out.print(newRunning->name());
		}

		newRunning->changeTo(Activity::RUNNING);

		dispatch(newRunning); // wechsle Coroutine
	}

	if (debug)
	{
		out.println();
	}
}

void ActivityScheduler::checkSlice()
{
	elapsedTicks += clock.ticks() - oldTicks;
	oldTicks = clock.ticks();

	Activity* running = (Activity*) active();

	if (elapsedTicks >= running->quantum())
	{
		reschedule();
		elapsedTicks = elapsedTicks - running->quantum();
	}
}
