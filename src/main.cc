#include <device/CgaChannel.h>
#include <io/PrintStream.h>

CgaChannel cga;		// unser CGA-Ausgabekanal
PrintStream out(cga);	// unseren PrintStream mit Ausgabekanal verknuepfen

int main()
{
	out.print(CgaScreen::Pages, 10);
	out.print(0x054F, 16);
	out.println();
	out.print(0x0, 16);
	out.println();
	out.print(-0x0, 16);
	out.println();
	out.print(-0x04583, 16);
	out.println();
	out.print((unsigned) 0xDEADBEEF, 16);
	out.println();
	out.print(0xBADEAFFE, 16);
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
	out.print((unsigned) 2147483648, 10);
	out.println();
	out.print(-2147483647, 10);
	out.println();

	out.print(0b1, 2);
	out.println();
	out.print(0b100, 2);
	out.println();
	out.print(0b00, 2);
	out.println();
	out.print((unsigned) -0b1001, 2);
	out.println();
	out.print(-0b1001, 2);
	out.println();
	out.print((unsigned) 0b10001000100010001000100010001000, 2);
	out.println();
	out.print(-0b000001110, 2);
	out.println();

	int i = 5;
	int* j = &i;
	out.print(j);
	out.println("jdfgj");
	out.println("jdfgj2");
	out.println("jdsdfafgj");
	out.println("jdsdfafgj");
	out.print("j\b\b");

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
