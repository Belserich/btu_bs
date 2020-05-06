//#include "io/PrintStream.h"
#include <device/CgaChannel.h>

CgaChannel cga;		// unser CGA-Ausgabekanal
//PrintStream out(cga);	// unseren PrintStream mit Ausgabekanal verknuepfen

int main()
{
	cga.blueScreen();
	cga.setCursor(0, 0);
	cga.write("HAI\rhsi\nHAI\b\b\b\b", 15);
	//out.println("Hello World");
	//
	//for(int i = 0; i < 1000; i++) {
	//	out.print("Dezimal ");
	//	out.print(i);
	//	out.print(" Binaer ");
	//	out.print(i, 2);
	//	out.print(" Hexadezimal ");
	//	out.print(i, 16);
	//	out.println();
	//}
	//
	//out.println("done");

	while(1) {}

	return 0;
}
