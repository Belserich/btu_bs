// Testprogramm fuer Coroutinen

#include "device/CgaChannel.h"
#include "thread/Coroutine.h"

// Eine Coroutinen-Klasse
//
// Anmerkung: Diese Klasse dient
// zu Testzwecken und der Einfacheit halber sind
// alle Methoden dieser Klasse ausnahmsweise inline deklariert
// Das sollte normalerweise *nicht* der Fall sein!

class Hello: public Coroutine {
public:
	Hello(const char* name, PrintStream& out)
		: cout(out)
	{
		this->name = name;
	}

	Hello(const char* name, PrintStream& out, void* sp)
		: Coroutine(sp), cout(out)
	{
		this->name = name;
	}

	void link(Coroutine& next)
	{
		this->next = &next;
	}

	void body()
	{
		for(int i=0; i<100; i++) {
			cout.print(name);
			cout.print(" ");
			cout.print(i);
			cout.println();

			resume(next);
		}
	}

	void exit()
	{
		cout.print("Panic!!!!");
		cout.println();
	}

	const char* name;
	Coroutine* next;

	PrintStream& cout;
};

//////////////////////////////////////////////////////////////////////////
// Die Systemobjekte von Co-Stubs

// globale Ein-/Ausgabeobjekte

CgaChannel cga;         // unser CGA-Ausgabekanal
PrintStream out(cga);   // unseren PrintStream mit Ausgabekanal verknuepfen

// die Stacks fuer unsere Prozesse/Coroutinen

unsigned stack0[1024];
unsigned stack1[1024];

/*
Name	Adresse
fun1 	0x0
fun2 	0x1
fun		0x100
*/

//int fun(); // jmp 0x100

int main()
{
	out.print(stack0);
	out.println();
	out.print(&stack0[1024]);
	out.println();

	Hello anton("Anton", out); // anton benutzt den Stack von main
	Hello berta("Berta", out, &stack0[1024]);
	Hello caesar("Caesar", out, &stack1[1024]);

	// alle Coroutinen zyklisch verketten
	anton.link(berta);
	berta.link(caesar);
	caesar.link(anton);

	// los geht's
	anton.body();
}

// 0x100
//int fun() {
//
//}
//
//int y() {
//	fun(); // jmp 0x100
//}