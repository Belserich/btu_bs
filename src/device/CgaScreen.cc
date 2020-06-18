#include "device/CgaScreen.h"
#include <cstring>
   
//Konstruktoren
CgaScreen::CgaScreen() : attr(CgaAttr()), index(IOPort8(0x3d4)), data(IOPort8(0x3d5)) {
    setAttr(CgaAttr());
};
//Konstruktor mit Attributen
CgaScreen::CgaScreen(CgaAttr attr) : attr(CgaAttr()), index(IOPort8(0x3d4)), data(IOPort8(0x3d5)) {
    this->setAttr(attr);  
};

/*
clear Methode

Setzt jedes Zeichen des Bilschrims auf 0 (nichts)

*/
void CgaScreen::clear() {
	//Grafikspeicheradresse
	char *CGA = (char *) CgaScreen::ADDRESS;
	char *pos;
	//Standardattribute (schwarz/weiss) als Byte holen
	char attr = CgaAttr().getByteAttr();
	//ganzen Bilschirm durchgehen
    for(int i = 0; i < CgaScreen::ROWS; i++) {
        for(int j = 0; j < CgaScreen::COLUMNS; j++) {
			//Position im Speicher berechnen
			pos = CGA +2 * (j + i * CgaScreen::COLUMNS);
			//Speicher beschreiben
			*pos = 0;
			*(pos+1) = attr;
        }
    }
	//Cursor and den Bilschirmanfang setzten
    setCursor(0, 0);
};
/*
Scroll methode

Scrollt den Bildschirm runter,
also setzt jede Zeile eins nach oben.
Die erste Zeile wird gelöscht und 
die letzte ist frei, weil es die neue Zeile ist
*/
void CgaScreen::scroll() {
	char c, attr;
	//Grafikspeicheradresse
	char *CGA = (char *) CgaScreen::ADDRESS;
	char *pos;
	//Alle Zeichen im Bilschrim außer der letzten Zeile	
    for (int i = 0; i < CgaScreen::ROWS - 1; i++) {
        for (int j = 0; j < CgaScreen::COLUMNS; j++) {
			//Speicherposition berechnen der Zeile darunter
			pos = CGA + 2 * (j + (i+1) * CgaScreen::COLUMNS);
			//Char und Attribute aus  dem Speicher lesen
			c = *pos;
			attr = *(pos + 1);
            //Cursor an aktuelle Stelle setzten und die ausgelesenen Werte dort schreiben
			setCursor(j, i);
			showAttr(c, attr);
        }
    }
    //laetzte Zeile frei machen
    for (int x = 0; x < CgaScreen::COLUMNS; x++) {
		setCursor(x, CgaScreen::ROWS - 1);
		show(0);
    }
	//Cursor an den Anfang der Zeile setzen
	setCursor(0, CgaScreen::ROWS-1);
};

/*
setCursor Methode

Setzt den Cursor and die übergebene Stelle
und schreibt diese in die entsprechenden Datenregister

*/
void CgaScreen::setCursor(int column, int row) {
	//Speicherindex berechenen
    int index = (column + row * CgaScreen::COLUMNS);
	//Über das Index- und Daten-Register schreiben
	//Cursor high
    IOPort8(CgaScreen::INDEX).write(14);
	//Bit verschiebung um 8 Bit Struktur zu erhalten -> aus 16 bit wird 8 bit
    IOPort8(CgaScreen::DATA).write(index >> 8);
	//Cursor low
    IOPort8(CgaScreen::INDEX).write(15);
    IOPort8(CgaScreen::DATA).write(index );
};

/*
getCursor Methode

Liesst die Cursor Position aus den ensprechnden Datenregistern
und setzt die übergebenen Werte

*/
void CgaScreen::getCursor(int& column, int& row) {
	//Index-Register beschreiben und aus Daten-Register lesen
    IOPort8(CgaScreen::INDEX).write(14);
    int cursorH = (int) IOPort8(CgaScreen::DATA).read();
    IOPort8(CgaScreen::INDEX).write(15);
    int cursorL = (int) IOPort8(CgaScreen::DATA).read();
	//Bitoperation zum Verschieben
    int index = (cursorH << 8)+cursorL;
	//Zeilen ausrechnen
    row = index / CgaScreen::COLUMNS;
	//Zeichen ausrechnen
    column = index % CgaScreen::COLUMNS;
	//ggf. Scrollen
	if(row >= (CgaScreen::ROWS)) {
		scroll();
		getCursor(column, row);
	}	
};
/*
show Methode

Zeig ein char an aktueller Position,
 mit uebergebenen Attributen
*/
void CgaScreen::show(char ch, const CgaAttr& attr) {
	//Attribute setzten
    this->attr.setAttr(attr);
    show(ch);
};
/*
showAttr Methode

schreib ein Char und das übergebenene Attribut,
in den Grafikspeicher

*/
void CgaScreen::showAttr(char ch, volatile unsigned char attr) {
	//Cursor Position holen
    int row, col;
    getCursor(col, row);
	//Grafikspeicheradresse
	char *CGA = (char *) CgaScreen::ADDRESS;
	char *pos;
	//Grafikschpeicherposition errechnen
	pos = CGA + 2*(col+row*80);	
	//Char und Attribute in den Speicher schreiben
	*pos = ch;
	*(pos + 1) = attr;
};

/*
show Methode

Schreibt das uebergebene Char in den Grafikspeicher

*/
void CgaScreen::show(char ch) {
	//Cursor Position holen
	int row, col;
    getCursor(col, row);
	char *CGA = (char *) CgaScreen::ADDRESS;
	char *pos;
	//Grafikspeicherposition ausrechnen
	pos = CGA + 2*(col+row*80);	
	//Grafikspeicher beschreiben mit Char und Attribut
	*pos = ch;
	*(pos + 1) = attr.getByteAttr();
	

};