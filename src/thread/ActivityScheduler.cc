// belserich on 20.05.20

#include "thread/ActivityScheduler.h"

extern PrintStream out;

void ActivityScheduler::suspend()
{
	Activity* running = (Activity*) active();
	running->sleep();
}

void ActivityScheduler::kill(Activity* activity)
{
//	out.print("Toete Aktivitaet ");
//	out.print(activity->name());

	activity->changeTo(Activity::State::BLOCKED);
	readylist.remove(activity);

	Coroutine* co = activity;
	if (co == active()) // ist die zu toetende Coroutine gerade am Laufen?
	{
//		out.println(" die gerade lief.");
		reschedule(); // neue Coroutine zum Laufen bringen
	}

//	out.println();
}

void ActivityScheduler::exit()
{
	kill((Activity*) active());
}

void ActivityScheduler::activate(Schedulable *to)
{
	Activity* oldRunning = (Activity*) active();
//	out.print("Lief gerade: ");
//	out.print(oldRunning->name());
//	out.println();

	if (!oldRunning->isBlocked() && !oldRunning->isZombie())
	{
		oldRunning->changeTo(Activity::READY);
		schedule(oldRunning);
	}

	if (to != nullptr)
	{
		Activity* newRunning = (Activity*) to;
//		out.print(" Laeuft jetzt: ");
//		out.print(newRunning->name());
//		out.println();

		newRunning->changeTo(Activity::RUNNING);
		dispatch(newRunning); // wechsle Coroutine
	}
}
