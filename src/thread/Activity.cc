#include <thread/ActivityScheduler.h>
#include "thread/Activity.h"

extern PrintStream out;

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
//	out.print("Wecke Aktivitaet ");
//	out.print(name());
//	out.printlt
//	();
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
//	out.print("Exit auf ");
//	out.print(this->name());
//	out.println(" aufgerufen");

	if (!isZombie())
	{
		for (Activity* parent = (Activity*) parents.dequeue(); parent != nullptr; parent = (Activity*) parents.dequeue())
		{
//		out.print("Wecke die Elternaktivitaet (eine fruehere Activity die irgendwann join aufgerufen hat) mit dem Namen ");
//		out.print(parent->name());
//		out.println();
			parent->wakeup();
		}

//		out.print("Toete Aktivitaet ");
//		out.println(name());
		scheduler.kill(this);
	}
}

/* Der aktuelle Prozess wird solange schlafen gelegt, bis der
 * Prozess auf dem join aufgerufen wird beendet ist. Das
 * Wecken des wartenden Prozesses �bernimmt exit.
 */
void Activity::join()
{
	if (!isZombie())
	{
		parents.enqueue((Activity*) scheduler.active());
		scheduler.suspend();
	}
}

Activity::~Activity()
{
	exit();
}
