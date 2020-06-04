// belserich on 02.06.20

#include "device/PIT.h"

extern PrintStream out;

PIT::PIT()
	: ctrl(CONTROL_PORT),
	data(DATA_PORT)
{}

PIT::PIT(int us)
	: ctrl(CONTROL_PORT),
	data(DATA_PORT)
{
	interval(us);
}

void PIT::interval(int us)
{
	int ticks = us * 1000 / TIME_BASE;

	out.print("Setting interrupt timer interval to ");
	out.print(us / 1000);
	out.print(" ms (");
	out.print(ticks);
	out.println(" ticks).");

	// Zaehlerauswahl: 00, Lesen/Schreiben: 11 (lsb dann msb), Modus: 010, Zaehlformat: 0
	int ctrlWord = 0b00110100;
	int lsb = ticks;
	int msb = ticks>>8;

	ctrl.write(ctrlWord);
	data.write(lsb);
	data.write(msb);
}
