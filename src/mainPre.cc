// Testprogramm fuer kooperative Threads

#include "device/CgaChannel.h"
#include "device/CPU.h"
#include "io/PrintStream.h"
#include "thread/Activity.h"
#include "thread/ActivityScheduler.h"
#include "device/PIC.h"
#include "device/Clock.h"
#include "interrupts/InterruptGuardian.h"
#include "interrupts/IntLock.h"

// Hello: Eine kooperative Aktivitaet
//
// Anmerkung: Diese Klasse dient
// zu Testzwecken und der Einfacheit halber sind
// alle Methoden dieser Klasse ausnahmsweise inline deklariert
// Das sollte normalerweise *nicht* der Fall sein!
class Hello: public Activity {
public:
	Hello(const char* name, PrintStream& out, int timeSlice)
		: Activity(name, timeSlice), cout(out)
	{
		this->name = name;
	}

	Hello(const char* name, PrintStream& out, void* sp, int timeSlice)
		: Activity(sp, name, timeSlice), cout(out)
	{
		this->name = name;
		wakeup();
	}

	~Hello()
	{
		static bool debug = false;

		if (debug)
		{
			cout.print("Destructor start ");
			cout.println(name);
		}

		join();

		if (debug)
		{
			cout.print("Destructor end ");
			cout.println(name);
		}
	}

	void body()
	{
		static bool test = false;
		if (test)
		{
			while (true)
			{
				{
					IntLock lock; // es geht darum Objekte aus sleep ueber Interrupts aufwecken zu koennen
					cout.print(name);
					cout.print("; ");
				}
				yield();
			}
		}
		else
		{
			for(int i=0; i<5; i++) {
				{
					IntLock lock;
					cout.print(name);
					cout.print(" ");
					cout.print(i);
					cout.println();
				}
				for(int j=0; j<10000; j++);
			}
		}
	}

private:
	const char* name;
	PrintStream& cout;
};

//////////////////////////////////////////////////////////////////////////
// Die Systemobjekte von Co-Stubs

// globale Ein-/Ausgabeobjekte
CgaChannel cga;         // unser CGA-Ausgabekanal
PrintStream out(cga);   // unseren PrintStream mit Ausgabekanal verknuepfen

CPU cpu;

InterruptGuardian interruptGuardian;
PIC pic;
int millis = 1;
int micros = 0;
Clock clock(millis * 1000 + micros);

// Objekte der Prozessverwaltung
ActivityScheduler scheduler;   // der Scheduler

// die Stacks fuer unsere Prozesse/Coroutinen
unsigned stack0[1024];
unsigned stack1[1024];

int main()
{
	Hello anton("Anton", out, 1); // anton benutzt den Stack von main
	Hello berta("Berta", out, &stack0[1024], 1);
	Hello caesar("Caesar", out, &stack1[1024], 1);

	cpu.enableInterrupts();
	anton.body();

	while (true)
	{
//		CPU::halt();
	}
//	cpu.disableInterrupts();
	return 0;
}

