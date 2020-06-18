#include "device/PIT.h"
#include "io/PrintStream.h"

extern PrintStream out;

PIT::PIT(){
	
};

PIT::PIT (int us) {
    interval(us);
};

void PIT::interval (int us) {
	int iv = (us/TIME_BASE) * 1000;
	//Steuerungs Wort
    IOPort8(CONTROL_PORT).write(0x34);
	//Zeit schreiben
    IOPort8(DATA_PORT).write(iv);
    IOPort8(DATA_PORT).write((iv >> 8));
};
