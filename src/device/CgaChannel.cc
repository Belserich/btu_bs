#include "../../include/device/CgaChannel.h"

#include <string>

//Konstruktor
CgaChannel::CgaChannel() {

};
//Konstruktor mit Attribute
CgaChannel::CgaChannel(const CgaAttr& attr) {
	//Atribute setzten
    setAttr(attr);
};

/*
Write methode

Gibt jeden einzelnes Zeichen,
an die show() Methode weiter

*/
int CgaChannel::write(const char* data, int size) {
    char c;
    //Alle Zeichen durchgegehen
    for(int i = 0; i < size; i++) {
        c = *(data+i);
        int row, col;
        getCursor(col, row);
        //Steuerungszeichen filtern
        if(c == '\n') {
			//neue Zeile oder scrollen
            if(row < (CgaScreen::ROWS-1)) {
                setCursor(0, row+1);
            } else {
                scroll();
            }
        } else if(c == '\r') {
			//wieder zum Anfang der Zeile
            setCursor(0, row);
        } else {
			//Zeichen weiter an show()
            show(c);
			//Cursor weiter setzten
			setCursor(col+1, row);
        }
    }
    return 0;
}
/*
BlueScreen Methode 

Gibt einen klassischen Bluescreen aus.
Gibt die übergebene Fehler-Nachricht aus in der Farbe weiß.

*/
void CgaChannel::blueScreen(const char* error){
	//blauer Hintergrund und weiße Schrift
	CgaAttr attr;
    attr.setBackground(CgaAttr::BLUE);
    attr.setForeground(CgaAttr::WHITE);
    attr.setBlinkState(false);
	//Attribute als Byte 
	char attrChar = attr.getByteAttr();
	//Start Adresse Graphikspeicher
	char *CGA = (char *) 0xb8000;
    char *pos;
	//ganzen Bildschirm durchgehen
	for(int i = 0; i < CgaScreen::ROWS; i++) {
		for(int j = 0; j < CgaScreen::COLUMNS; j++) {
			//Position des Zeichen errechnen (fortlaufend gezaehlt)
			pos = CGA + 2*(j+i*80);	
			//Char und Attribute and die Speicheradresse setzten
			*pos = 0;
			*(pos + 1) = attrChar;
		}		
	}
	//Mitte des Bilschirms
	int x = 30;
	int y = 10;
	//Laenge der Fehlermeldung
	int length = 0;
	char *c;
	c = (char *)error;
	
	while (*c != '\0') {
        length++;
        c++;
    }
	//String durchgehen und Zeichen in den Speicher setzten
	for(int i = 0; i < length; i++) {
		pos = CGA + 2 * ((x+i) + y * 80);
		*pos = *(error + i);
		*(pos + 1) = attrChar;
	}
};




