#include "io/PrintStream.h"

PrintStream::PrintStream(OutputChannel* chan)
	: channel(*chan)
{}

PrintStream::PrintStream(OutputChannel& chan)
	: channel(chan)
{}

void PrintStream::print(const char* str)
{
	int size = 0;
	while (str[size] != '\0') // sucht nach dem NULL Steuerzeichen (jeder String wird mit so einem Zeichen beendet)
	{
		++size;
	}
	channel.write(str, size);
}

void PrintStream::print(char ch)
{
	channel.write(ch);
}

void PrintStream::println(const char* str)
{
	print(str);
	println();
}

void PrintStream::println()
{
	channel.write('\n');
	channel.write('\r');
}

void PrintStream::print(int x, int base)
{
	if (base == BINARY)
	{
		bool hasOne = false; // wird true nach Auffinden der ersten Eins

		if (x < 0)
		{
			print("-0b");
			x = -x;
		}
		else print("0b");

		// i ist der shift offset (30, bei 32-bit ints), wir beginnen bei 30 (vorletzter Index im Bitstring der Zahl), da der letzte das Vorzeichen angibt
		for (int i = sizeof(int) * 8 - 2; i >= 0; --i)
		{
			int currBit = (x >> i) & 1; // schiebt das zu betrachtende Bit an die erste Position und loescht mit & 1 alle anderen Positionen

			// currBit kann nur 1 oder 0 sein
			if (currBit) // 1 steht immer fuer wahr und 0 immer fuer falsch, daher kann man das hier so schreiben
			{
				if (!hasOne)
				{
					hasOne = true; // auschlaggebend dafuer, ob eine null auch im string als 0 gesetzt wird. Ist fuer alle Nullen nach der letzten Eins nicht noetig
				}
				print("1");
			}
			else if (hasOne || (i == 0 && !hasOne)) // currBit = 0 oder keine Eins im Bitstring (in diesem Fall ist die Zahl Null und mind. eine Null wird geschrieben
			{
				print("0");
			}
		}
	}
	else if (base == DECIMAL)
	{
		bool has = false;

		if (x < 0)
		{
			print("-");
			x = -x; // konvertiert eine negative Zahl im Zweierkomplement in ihre positive Darstellung
		}

		// im Folgenden betrachten wir jede einzelne Zahl (des Dezimalstrings) von links nach rechts
		for (int i = 9; i >= 0; --i) // ist die Zehnerpotenz durch die wir den Dezimalwert teilen
		{
			int num = 1;
			for (int j = 0; j < i; ++j) num *= 10; // berechnet 10^i

			int quot = x / num;

			if (quot > 0)
			{
				has = true;

				print((char) (48 + quot)); // Zahl von 0-9
			}
			else if (has || i == 0)
			{
				print('0');
			}

			x -= (x / num) * num; // ziehen die gefundene Zahl vom Dezimalstring ab (Bsp.: 113 / 100 = 1 (linke Ziffer) -> x -= ... = 113 - 100 = 13)
		}
	}
	else if (base == HEX)
	{
		// hier das selbe Spiel wie bei BINARY
		bool has = false; // wird true nach Auffinden der ersten Zahl ungleich Null

		if (x < 0)
		{
			print("-0x");
			x = -x; // konvertiert eine negative Zahl im Zweierkomplement in ihre positive Darstellung
		}
		else print("0x");

		// i ist der Offset-Multiplikator fuers spaetere bit shifting. Fuer 32bit ints geht i vom 8. Halbbyte bis zum 1. (Indizes 7-0)
		for (int i = sizeof(int) * 2 - 1; i >= 0; --i)
		{
			int currNib = (x >> i * 4) & 0xf; // schiebt das zu betrachtende Halbbyte (Nibble) an die erste Position und loescht mit & 0xf alle anderen Positionen

			// currBit kann nur einen von 16 Werten annehmen
			if (currNib > 0)
			{
				has = true; // auschlaggebend dafuer, ob eine Null auch im string als 0 gesetzt wird. Ist fuer alle Nullen nach der letzten Eins nicht noetig

				if (currNib <= 9)
				{
					print((char) (48 + currNib)); // 48 ist die 0 im ASCII-code, der char fuer i liegt hat also den Wert 48 + i mit 0 <= i <= 9
				}
				else
				{
					print((char) (65 + (currNib - 10))); // selbes Spiel wie oben, die 65 ist das A und wir ziehen 10 ab, weil die ersten 10 Halbbytes als Zahlen darstellbar sind
				}
			}
			else if (has || i == 0) // currNib == 0 oder keine Eins im Bitstring (in diesem Fall ist die Zahl Null und mind. eine Null wird geschrieben
			{
				print('0');
			}
		}
	}
}

void PrintStream::print(unsigned int x, int base)
{

}

void PrintStream::print(void* p)
{
//	int** addr = (int**) &p;
//	print(addr, HEX);
}
