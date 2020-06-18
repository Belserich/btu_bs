#include "io/PrintStream.h"
#include "device/CgaChannel.h"


//Konstruktoren mit Channel
PrintStream::PrintStream(OutputChannel *chan)
        : channel(*chan) {};

PrintStream::PrintStream(OutputChannel &chan)
        : channel(chan) {};

		
/*
Print Methode

Schreibt den String 

*/
void PrintStream::print(const char* str) {
    int i= 0;
	char *c;
	c = (char *)str;
	//Länge des String berechnen
	while (*c != '\0') {
        i++;
        c++;
    }
    channel.write(str, i);
};

/*
Print Methode

Schreib ein einzelnes Zeichen

*/
void PrintStream::print(char ch) {
    channel.write((char *) &ch, 1);
};

/*
PrintLine Methode

Schreibt den String und 
geht dann in die naechste Zeile

*/
void PrintStream::println(const char* str) {
    print(str);
    println();
};

/*
PrintLine Methode

Schreib über Steuerungszeichen eine neue Zeile

*/
void PrintStream::println() {
    channel.write('\n');
};

/*
Print Methode für Zahlen

Die Methode schreib Zahlen, je nach der Basis,
standard Basis ist 10.

*/
void PrintStream::print(int x, int base) {
	if((base != 2) && (base != 10) && (base != 16)) {
		print("Falsche Basis");
		return;
	}
	char ch[34];
	bool neg = false;
	//Wenn negativ - praefix
	if(x < 0) {
		neg = true;
		x = -(x);
	}
	
	//Laenge des Integers
	int len = 1;
	int temp_num = x;
	if (temp_num > 0) {
		for (len = 0; temp_num > 0; len++) {
			temp_num /= base;
		}
	}
	int idx = len-1;;
	
	if(x == 0) {
		ch[0] = 48;
	}
	//Umrechnung  mit der Basis
	while(x > 0)
	{
		//48 für Ascii Zahlenraum
		ch[idx] = (x%base) + 48;
		x /= base;
		idx --;         
	}
	//Wenn hex 
	if(base == 16) {
		//Hex  praefix
		print("0x");
		if(neg) {
			channel.write('-');
		}
		for(int i = 0; i < len; i++) {
			//Wenn zahl ueber 10 dann Alphabet
			if(ch[i]>57) {
				ch[i]+=7;
			}
			channel.write(ch[i]);
		}
	} else {
		//Binaer praefix
		if(base == 2) {
			print("0b");
		}
		if(neg) {
			channel.write('-');;
		}
		//Jedes Zeichen einzeln schreiben
		for(int i = 0; i < len; i++) {
			channel.write(ch[i]);
		}
	}
};

/*
Print Methode für Zahlen mit unsigned

Die Methode schreibt Zahlen,
mit der zugehörigen Basis,
standard Basis ist 10.
Nur positive Werte.


*/
void PrintStream::print(unsigned x, int base) {
	if(base != 2 && base != 10 && base != 16) {
		print("Falsche Basis");
		return;
	}
	//Umwandlung  zum int
	unsigned int num = (unsigned int) x;
	char ch[64];
	//Keine neg. Zahlen
	if(num < 0) {
		//Error Nachricht
		print("Fehler Keine neg. Zahlen bei unsigned;");
		return;
	}
	//Laenge des Ints ermitteln
	int len = 1;
	unsigned int temp_num = num;
	if (temp_num > 0) {
		for (len = 0; temp_num > 0; len++) {
			temp_num /= base;
		}
	}
	int idx = len-1;;
	
	if(num == 0) {
		ch[0] = 48;
	}
	//Wert mit der Basis verrechnen
	while(num > 0)
	{
		//48 addieren für Ascii Zahlenraum
		ch[idx] = (num%base) + 48;
		num /= base;
		idx --;         
	}
	//Wenn HEx
	if(base == 16) {
		//Praefix für Hex
		print("0x");
		for(int i = 0; i < len; i++) {
			//Wenn zahl ueber 10 dann Alphabet
			if(ch[i]>57) {
				ch[i]+=7;
			}
			channel.write(ch[i]);
		}
	} else {
		//Praefix für binaer
		if(base == 2) {
			channel.write('B');
		}
		//Jedes Zeichen einzeln ausgeben
		for(int i = 0; i < len; i++) {
			channel.write(ch[i]);
		}
	}
};

/*
Print Methode mit Zeigern

Diese Methode gibt den Speicher,
des übergebenen Zeigers wieder,
das als Hex-Wert


*/
void PrintStream::print(void* p){
	//Zeiger als int
	int num = (int) p;
	char ch[34];
	//Länge vom Zeiger
	int len = 1;
	int temp_num = num;
	if (temp_num > 0) {
		for (len = 0; temp_num > 0; len++) {
			temp_num /= 16;
		}
	}
	//Index für Char
	int idx = len-1;;
	
	if(num == 0) {
		ch[0] = 48;
	}
	//Umwandlung in Hex mit Basis 16
	while(num > 0)
	{
		ch[idx] = (num%16) + 48;
		num /= 16;
		idx --;         
	}
	//Hex Pre-Fix
	print("0x");
	//Jedes Zeichen schreiben
	for(int i = 0; i < len; i++) {
		//Alles ab 10 als Buchstabe
		if(ch[i]>57) {
			ch[i]+=7;
		}
		channel.write(ch[i]);
	}
};