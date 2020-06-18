#ifndef CgaChar_h
#define CgaChar_h

#include "device/CgaAttr.h"

/*
 * CgaChar:
 * 
 *	Diese Klasse stellt den Prototyp eines
 *	Zeichens des Bildschirms dar.
 *	Dieses besteht aus darstellbarem Zeichen
 *	und Darstellungsattributen.
 */


class CgaChar {

public:

	// setzen des Zeichens
	void setChar(char c) {
		this->character = c;
	};
	
	// auslesen des Zeichens
	char getChar() {
		return character;
	};
	
	// setzen der Darstellungsattribure
	void setAttr(const CgaAttr& attr){
		CAttr.setAttr(attr);
	};
	
	// auslesen der Darstellungsattribute
	CgaAttr getAttr() {
		return CAttr;	
	};

private:
	CgaAttr CAttr;
	unsigned char character;

};

#endif

