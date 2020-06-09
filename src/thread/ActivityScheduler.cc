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
	IntLock lock;
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
	IntLock lock; // es wird aufs aktive Element in der Queue zugegriffen

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
//		out.print("Queue: ");
//		printQueue();
	}

	activity->changeTo(Activity::State::ZOMBIE);
	Activity* act = (Activity*) active();
	if (activity == act) // ist die zu toetende Coroutine gerade am Laufen?
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
	IntLock lock;
	Activity* running = (Activity*) active();
	running->changeTo(Activity::ZOMBIE);
	reschedule();
}

void ActivityScheduler::activate(Schedulable *to)
{
	static bool debug = false;
	IntLock lock; // falls activate() direkt von aussen aufgerufen wird

	if (to != nullptr)
	{
		Activity* oldRunning = (Activity*) active();
		Activity* newRunning = (Activity*) to;

		if (debug)
		{
			out.print("Lief gerade: ");
			out.print(oldRunning->name());
			if (!oldRunning->isBlocked() && !oldRunning->isZombie())
			{
				out.print(" (neu eingereiht)");
			}
			out.print("; Laeuft jetzt: ");
			out.print(newRunning->name());
			out.println();
		}

		if (!oldRunning->isBlocked() && !oldRunning->isZombie())
		{
			oldRunning->wakeup();
		}

		newRunning->changeTo(Activity::RUNNING);
		dispatch(newRunning); // wechsle Coroutine
	}
}

//void ActivityScheduler::printQueue()
//{
//	IntLock lock;
//	Activity* next;
//	Queue temp;
//	for (next = (Activity*) readylist.dequeue(); next != nullptr; next = (Activity*) readylist.dequeue())
//	{
//		temp.enqueue(next);
//		out.print(next->name());
//		out.print(" ");
//	}
//	out.println();
//	for (next = (Activity*) temp.dequeue(); next != nullptr; next = (Activity*) temp.dequeue())
//	{
//		readylist.enqueue(next);
//	}
//}

void ActivityScheduler::checkSlice()
{
	IntLock lock; // sollen ja immer eine ganze Funktion abdecken. Lock noetig, da checkSlice() public.
	elapsedTicks += clock.ticks() - oldTicks;
	oldTicks = clock.ticks();

	Activity* running = (Activity*) active();
	if (running != nullptr)
	{
		if (elapsedTicks >= running->quantum())
		{
			elapsedTicks = elapsedTicks - running->quantum();
			reschedule();
		}
	}
	else
	{
		CPU::enableInterrupts();
		CPU::halt();
		CPU::disableInterrupts();
	}
}
