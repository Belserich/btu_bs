#ifndef CgaScreen_h
#define CgaScreen_h

#include "device/CgaAttr.h"
#include "device/CgaChar.h"
#include "io/IOPort.h"

/*
 * CgaScreen:	Diese Klasse ist der Softwareprototyp fuer den
 *		CGA-Bildschirm
 *		
 *      Hier soll es ausschliesslich um die Kontrolle der Hardware
 *      gehen. Komplexere Aufgaben koennen in den erbenden Klassen
 *      implementiert werden.
 */


class CgaScreen {

private:

	// Die I/O-Ports des Grafikcontrollers
	enum Ports  {
        P1 = 0x3D4,
        P2 = 0x3D5
	};

	// Die Kommandos zum Cursor setzen
	enum Cursor {
        I1 = 14,
        I2 = 15
	};

public:
	// Die Bildschirmdimensionen
	enum Screen {
		Rows = 25,
		Columns = 80
	};

	// Die Adresse des Video RAMs
	// Offset 0 im Video-RAM wird mit (hexadezimal) b8000 adressiert
	enum Video  {
		Offset0 = 0xb8000
	};

	// Standardattribute waehlen und Bildschirm loeschen
    /* attr2 initialisiert, keine Parameter übergeben, also Defaultwerte verwendet */
	CgaScreen();    // default constructor

	// Angegebene Attribute setzen und Bildschirm loeschen
	explicit CgaScreen(CgaAttr attr);

	// Loeschen des Bildschirms
    // Offset0 + i * 2      -> 1.Byte
    // Offset0 + i * 2 + 1  -> 2.Byte
	void clear ();

	// Verschieben des Bildschirms um eine Zeile
	void scroll();

	// Setzen/Lesen der globalen Bildschirmattribute
    // Ab hier werden References genutzt, also am Typ hängt ein &
	void setAttr(const CgaAttr& attr);

	void getAttr(CgaAttr& attr);

	// Setzen/Lesen des HW-Cursors
	void setCursor(int column, int row);
    
	void getCursor(int& column, int& row);

	// Anzeigen von c an aktueller Cursorposition
	// Darstellung mit angegebenen Bildschirmattributen
	void show(char ch, const CgaAttr& attr);

	// Anzeigen von c an aktueller Cursorposition
    	// Darstellung mit aktuellen Bildschirmattributen
	void show(char ch);

protected:

	CgaAttr mAttr;
	IOPort8 index;
	IOPort8 data;
	CgaChar* screen;
};

#endif
