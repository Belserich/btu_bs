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
    
	// Die Adresse des Video RAMs
    // Offset 0 im Video-RAM wird mit (hexadezimal) b8000 adressiert
	enum Video  {
        Offset0 = 0xb8000
	};

public:
	// Die Bildschirmdimensionen
	enum Screen {
		ROWS = 25,
		COLUMNS = 80
	};

	// Standardattribute waehlen und Bildschirm loeschen
    /* attr2 initialisiert, keine Parameter übergeben, also Defaultwerte verwendet */
	CgaScreen()    // default constructor
    {
        CgaAttr attr2;
        clear();
    };

	// Angegebene Attribute setzen und Bildschirm loeschen
	explicit CgaScreen(CgaAttr attr)
    {
        setAttr(attr);
        clear();
    };

	// Loeschen des Bildschirms
    // Offset0 + i * 2      -> 1.Byte
    // Offset0 + i * 2 + 1  -> 2.Byte
	void clear ()
    {
        CgaAttr cAttr(Color BLACK, Color BLACK, false);
        int j = 1;
        
        for(int i = 0; i <= 79; i++)
        {
            setCursor(i, j);
            setAttr(cAttr);
            
            if(i == 79) 
            {
                i = 1;
                j++;
            } 
            
            if(j == 26)
            {
                i = 81;
            }
            
        }
    };

	// Verschieben des Bildschirms um eine Zeile
	void scroll()
    {
        int j = 2;
        
        for(int i = 0; i <= 79; i++)
        {
            setCursor(i, j - 1);
            setAttr(getCursor(i, j));
            
            if(i == 79)
            {
                i = 1;
                j++;
            }
            
            if(j == 26)
            {
                i = 81;
            }
        }
    };

	// Setzen/Lesen der globalen Bildschirmattribute
    // Ab hier werden References genutzt, also am Typ hängt ein &
	void setAttr(const CgaAttr& attr)
	{
        this->attr = attr;
	}

	void getAttr(CgaAttr& attr)
	{
        attr = this->attr;
	}

	// Setzen/Lesen des HW-Cursors
	void setCursor(int column, int row)
    {
        int adresse = row * COLUMNS + column;
       
        index.outb(Ports P1, Cursor I2);
        data.outb(Ports P2, adresse & 0xff);    // Low Byte
        index.outb(Ports P1, Cursor I1);
        data.outb(Ports P2, (adresse >> 8) & 0xff); //High Byte
    };
    
	void getCursor(int& column, int& row)
    {
        data.inb(Ports P2);
    };


	// Anzeigen von c an aktueller Cursorposition
    	// Darstellung mit angegebenen Bildschirmattributen
	void show(char ch, const CgaAttr& attr)
    {
        index.outb(Ports P1, Cursor I2) = ch;
        data.outb(Ports P2, Cursor I1) = attr;
    };

	// Anzeigen von c an aktueller Cursorposition
    	// Darstellung mit aktuellen Bildschirmattributen
	void show(char ch)
	{
        index.outb(Ports P1, Cursor I2) = ch;
        data.outb(Ports P2, Cursor I1) = this->attr;
	}


protected:

	CgaAttr attr;
	IOPort8 index;
	IOPort8 data;
	CgaChar* screen;
};

#endif
