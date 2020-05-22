// Testprogramm fuer kooperative Threads

#include "device/CgaChannel.h"
#include "io/PrintStream.h"
#include "thread/Activity.h"
#include "thread/ActivityScheduler.h"

// Hello: Eine kooperative Aktivitaet
//
// Anmerkung: Diese Klasse dient
// zu Testzwecken und der Einfacheit halber sind
// alle Methoden dieser Klasse ausnahmsweise inline deklariert
// Das sollte normalerweise *nicht* der Fall sein!
class Hello: public Activity {
public:
	Hello(const char* name, PrintStream& out, int count=10)
		: Activity(name), cout(out)
	{
		this->name = name;
		this->count = count;
	}

	Hello(const char* name, PrintStream& out, void* sp, int count=10)
		: Activity(name, sp), cout(out)
	{
		this->name = name;
		this->count = count;
		wakeup();
	}
	
	~Hello()
	{
	}

	void body()
	{
		for(int i=0; i<=count; i++) {
			cout.print(name);
			cout.print(" ");
			cout.print(i);
			cout.println();

			yield();
		}
	}

protected:
	const char* name;
	PrintStream& cout;
	int count;
};

class Hello2 : public Hello
{
public:
	Hello2(const char *name, PrintStream &out, void *sp, void *sp2, int count = 10)
			: Hello(name, out, sp, count), delta("kind_delta", cout, sp2, 1)
	{}

	void body()
	{
		cout.println("Eintritt in body() von ");
		cout.println(name);
		cout.println("... gefolgt vom Eintritt in den Destruktor");

		delta.join();
		cout.println("Eine Zeile hinter join in Berta's Destruktor().");
	}

private:
	Hello delta;
};

//////////////////////////////////////////////////////////////////////////
// Die Systemobjekte von Co-Stubs

// globale Ein-/Ausgabeobjekte
CgaChannel cga;         // unser CGA-Ausgabekanal
PrintStream out(cga);   // unseren PrintStream mit Ausgabekanal verknuepfen

// Objekte der Prozessverwaltung
ActivityScheduler scheduler;   // der Scheduler

// die Stacks fuer unsere Prozesse/Coroutinen
unsigned stack0[1024];
unsigned stack1[1024];
unsigned stack2[1024];

int main()
{
	Hello anton("Anton", out, 3); // anton benutzt den Stack von main
	Hello2 berta("Berta", out, &stack0[1024], &stack2[1024], 10);
//	Hello caesar("Caesar", out, &stack1[1024], 15);

	anton.body();
}
