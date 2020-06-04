#include <thread/ActivityScheduler.h>
#include "thread/Activity.h"

extern PrintStream out;

Activity::Activity(void *tos, const char* name, int timeSlice)
	: Schedulable(timeSlice), Coroutine(tos), mName(name), isMain(false)
{}

Activity::Activity(const char* name, int timeSlice)
	: Schedulable(timeSlice), Coroutine(), mName(name), isMain(true)
{
	scheduler.start(this); // setzt die laufende Activity
}

/* Veranlasst den Scheduler, diese Aktivitaet aufzuwecken.
 */
void Activity::wakeup()
{
	IntLock lock;

	changeTo(READY);
	scheduler.schedule(this);
}

/* Diese Aktivitaet gibt die CPU vorruebergehend ab.
 */
void Activity::yield()
{
	IntLock lock;

	changeTo(READY);
	scheduler.reschedule();
}

/* Veranlasst den Scheduler, diese Aktivitaet zu suspendieren.
 */
void Activity::sleep()
{
	IntLock lock;

	scheduler.suspend();
}

/* Diese Aktivitaet wird terminiert. Hier muss eine eventuell
 * auf die Beendigung wartende Aktivit�t geweckt werden.
 */
void Activity::exit()
{
	IntLock lock;

	if (!isZombie())
	{
		for (Activity* parent = (Activity*) parents.dequeue(); parent != nullptr; parent = (Activity*) parents.dequeue())
		{
			parent->changeTo(READY);
			scheduler.schedule(parent);
		}

		scheduler.kill(this);
	}
}

/* Der aktuelle Prozess wird solange schlafen gelegt, bis der
 * Prozess auf dem join aufgerufen wird beendet ist. Das
 * Wecken des wartenden Prozesses �bernimmt exit.
 */
void Activity::join()
{
	IntLock lock;

	Activity* parent = (Activity*) scheduler.active();
	if (!isZombie() && parent != this)
	{
		parents.enqueue(parent);
		scheduler.suspend();
	}
}

Activity::~Activity()
{
	exit();
	if (isMain)
	{
		CPU::disableInterrupts();
	}
}
