#include <thread/ActivityScheduler.h>
#include "thread/Activity.h"

extern PrintStream out;
extern bool debugFlag;

Activity::Activity(const char* name, void *tos)
	: Coroutine(tos), mName(name)
{}

Activity::Activity(const char* name)
	: Coroutine(), mName(name)
{
	scheduler.start(this);
}

/* Veranlasst den Scheduler, diese Aktivitaet aufzuwecken.
 */
void Activity::wakeup()
{
	changeTo(READY);
	scheduler.schedule(this);
}

/* Diese Aktivitaet gibt die CPU vorruebergehend ab.
 */
void Activity::yield()
{
	changeTo(READY);
	scheduler.reschedule();
}

/* Veranlasst den Scheduler, diese Aktivitaet zu suspendieren.
 */
void Activity::sleep()
{
	changeTo(BLOCKED);
	scheduler.remove(this);
}

/* Diese Aktivitaet wird terminiert. Hier muss eine eventuell
 * auf die Beendigung wartende Aktivit�t geweckt werden.
 */
void Activity::exit()
{
	if (!isZombie())
	{
		for (Activity* parent = (Activity*) parents.dequeue(); parent != nullptr; parent = (Activity*) parents.dequeue())
		{
			if (debugFlag)
			{
				out.print("Wecke die Elternaktivitaet ");
				out.print(parent->name());
				out.println();
			}
			parent->wakeup();
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
}
