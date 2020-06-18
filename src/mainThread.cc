
#include "device/CgaChannel.h"
#include "io/PrintStream.h"
#include "thread/Activity.h"
#include "thread/ActivityScheduler.h"


class Villian: public Activity {
public:
    Villian(const char* name, PrintStream& out, int count=10)
            : cout(out)
    {
        this->name = name;
        this->count = count;
    }

    Villian(const char* name, PrintStream& out, void* sp)
            : Activity(sp), cout(out)
    {
        this->name = name;
        wakeup();
    }

    ~Villian()
    {
        join();
    }

    void body()
    {
			wakeup();
			cout.print(name);
            cout.print(": ");
			if(name == "El Patron") {
				cout.println("Ich habe ein unmoralisches Angebot fuer dich?");
				sleep();
				cout.print(name);
				cout.println(": Wenn du ein Problem hat dann kommst du zu mir?");	
			} else if(name == "Mephisto"){
				cout.println("Ich bin ein Teil von jener Kraft, die stets das Boese will und stets das Gute schafft");
				yield();
				cout.print(name);
				cout.println(": Das also war des Pudels Kern!");
			} else if(name == "Keyser Soze"){
				cout.println("Ich glaube, dies ist der Beginn einer wunderbaren Freundschaft.");
				join();
				cout.print(name);
				cout.println(": Der groesste Trick des Teufels, mal abseits seiner Plaene, war es die Menscheit glauben lassen, das es ihn nicht gaebe.");
				exit();
			}
            
            cout.println();
			
    }

private:
    const char* name;
    PrintStream& cout;
    int count;
};


CgaChannel cga;         // unser CGA-Ausgabekanal
PrintStream out(cga);   // unseren PrintStream mit Ausgabekanal verknuepfen

// Objekte der Prozessverwaltung
ActivityScheduler scheduler;   // der Scheduler


unsigned stack0[1024];
unsigned stack1[1024];
int main()
{
    Villian patron("El Patron", out); 
    Villian mephisto("Mephisto", out, &stack0[1024]);
	Villian keyser("Keyser Soze", out, &stack1[1024]);
	
    patron.body();
}

