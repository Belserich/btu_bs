#include "device/Clock.h"

/* Clock konstruktoren
	setzt ticks auf 0 und je nach übergabe das Uhreninterval
	intialisiert Gate mit Imer und PIT

*/
Clock::Clock() : Gate(InterruptVector::Timer), PIT() {
    this->tick = 0;
};

Clock::Clock(int us) : Gate(InterruptVector::Timer), PIT(us) {
    this->tick = 0;
    windup(us);
};

/*	windup Methode
initialisiert Ticks und pit 

*/
void Clock::windup(int us) {
	//Interval setzten
	interval(us);
	pic.enable(PIC::PIT);
};

/* handle Methode
Wird jeden tick aufgerufen

*/
void Clock::handle() {
	//Pic bestätigen
    pic.ack();
	/*
	if(tick%200 == 0 ){
      out.print("\r|");
      tick = 0;
	} else if(tick%150 == 0) {
		out.print("\r\\");
	} else if(tick%100 == 0) {
		out.print("\r-");
	} else if(tick%50 == 0) {
		out.print("\r/");
	}
    tick++;
	*/
	//out.print("Check");
	//Zeitscheiben überprüfen
    scheduler.checkSlice();
};
