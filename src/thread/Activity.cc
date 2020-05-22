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

void Activity::wakeup()
{
	changeTo(READY);
	scheduler.schedule(this);
}

void Activity::yield()
{
	changeTo(READY);
	scheduler.reschedule();
}

void Activity::sleep()
{
	changeTo(BLOCKED);
	scheduler.reschedule();
}

void Activity::exit()
{
	if (exited)
	{
		return;
	}

	out.print("Exit auf ");
	out.print(this->name());
	out.println(" aufgerufen");

	if (parent != nullptr)
	{
		out.println("Wecke die Elternaktivitaet (eine fruehere Coroutine die irgendwann join aufgerufen hat)");
		parent->wakeup(); // parent->wakeup() eigentlich aber parent wurde ja auf den nullptr gesetzt
		parent = nullptr;
	}

	exited = true;
	changeTo(BLOCKED);
	scheduler.kill(this);
}

void Activity::join()
{
	changeTo(ZOMBIE);
	parent = (Activity*) scheduler.active();
	scheduler.suspend();
	/**
	 * Joiner -> ruft join() auf einer Coroutine auf
	 * Joined -> die Coroutine, auf der durch Joiner join() aufgerufen wurde
	 * -------
 	 * Potentielle Fehlerquelle: Da hier nur ein joiner gespeichert wird treten Probleme auf, wenn im Kreis gejoint wird.
 	 * Beispiel: Cor. 1 - join() > Cor. 2 - join() > Cor. 3 - join() > Cor. 1
	 *
 	 */
}

Activity::~Activity()
{
	exit();
}
