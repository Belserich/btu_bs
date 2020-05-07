#include <device/CgaChannel.h>
#include <io/PrintStream.h>

CgaChannel cga;		// unser CGA-Ausgabekanal
PrintStream out(cga);	// unseren PrintStream mit Ausgabekanal verknuepfen

int main()
{
//	cga.blueScreen();
//	cga.setCursor(0, 0);
//	cga.write("HAI\rhsi\nHAI\b\b\b\b", 15);
	out.print(0x054F, 16);
	out.println();
	out.print(0x0, 16);
	out.println();
	out.print(-0x0, 16);
	out.println();
	out.print(-0x04583, 16);
	out.println();
	out.print(0xDEADBEE, 16);
	out.println();

	out.print(1000, 10);
	out.println();
	out.print(1145, 10);
	out.println();
	out.print(38475302, 10);
	out.println();
	out.print(0, 10);
	out.println();
	out.print(-0, 10);
	out.println();
	out.print(-3, 10);
	out.println();
	out.print(-34578, 10);
	out.println();

	out.print(0b1, 2);
	out.println();
	out.print(0b100, 2);
	out.println();
	out.print(0b00, 2);
	out.println();
	out.print(-0b1001, 2);
	out.println();
	out.print(-0b000001110, 2);
	out.println();

//	for(int i = 0; i < 1000; i++) {
//		out.print("Dezimal ");
//		out.print(i);
//		out.print(" Binaer ");
//		out.print(i, 2);
//		out.print(" Hexadezimal ");
//		out.print(i, 16);
//		out.println();
//	}

	out.println("done");

	while(1) {}

	return 0;
}
