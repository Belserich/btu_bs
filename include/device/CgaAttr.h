#ifndef CgaAttr_h
#define CgaAttr_h

/*
 * CgaAttr: 	Softwareprototyp fuer CGA Darstellungsattribute
 *		Hier braucht man Bitoperationen!
 *
 *		Da *alle* Operationen sehr einfach und kurz sind,
 *		duerft Ihr sie direkt in der Klasse
 *		als inline Methoden deklarieren.
 */


class CgaAttr {
private:
	enum AttrMaskAndShifts {

		/* Darstellungsattriubute:
		*	Vordergrundfarbe: Bits 0-3
		*	Hintergrundfarbe: Bits 4-6
		*	Blinken: Bit 7
		*/

		ForegroundColor = 0b00001111,
		BackgroundColor = 0b01110000,
		Blink = 0b10000000,

	};


public:
	/** 	Diese Aufz�hlung enth�lt die CGA-Farben als Farbkonstanten
	 *	Tragt hier *alle* CGA Farben mit den richtigen Werten ein
	 */
	enum Color {
		BLACK = 0b000,
		BLUE = 0b001,
		BROWN = 0b110,
		CYAN = 0b011,
		GRAY = 0b1000,
		GREEN = 0b010,
		LIGHT_BLUE = 0b1001,
		LIGHT_CYAN = 0b1011,
		LIGHT_GRAY = 0b111,
		LIGHT_GREEN = 0b1010,
		LIGHT_MAGENTA = 0b1101,
		LIGHT_RED = 0b1100,
		MAGENTA = 0b101,
		RED = 0b100,
		WHITE = 0b1111,
		YELLOW = 0b1110
	};


	/** Konstruktor. Erzeugt ein CgaAttr-Objekt mit den uebergebenen Werten f�r
	  * Vorder- und Hintergrundfarbe. Werden keine Parameter uebergeben,
	  * so werden die Defaultwerte (Vordergrund weiss, Hintergrund schwarz, Blinken deaktiviert)
	  * verwendet.
	  */
	CgaAttr(Color fg = WHITE, Color bg = BLACK, bool blink = false)
	{
		setForeground(fg);
		setBackground(bg);
		setBlinkState(blink);
	}

	/* setzen der Schriftfarbe
	* col muss nicht geshiftet werden, Form ist bereits richtig: 0b 0000 xxxx
	* mit der Farbe (col) �ndert man die ersten 4 Bits
	* ((~ForegroundColor) & this->x) sind die restlichen Bits des Attributs x die nicht verloren gehen sollen
	*/
	void setForeground(Color col)
	{
		x = col | ((~ForegroundColor) & this->x);
	}

	/*
	* setzen der Hintergrundfarbe
	* shiften um 4 Bits nach Links f�r die Form: 0b 0xxx 0000
	*/
	void setBackground(Color col)
	{
		this->x = ((col<<4) & BackgroundColor) | ((~BackgroundColor) & this-> x);
	}

	/*
	* setzen blinkender/nicht blinkender Text
	* shiften um 7 Bits nach Links f�r die Form: 0b x000 0000
	*/

	void setBlinkState(bool blink)
	{
		x = blink ? x | Blink : x & ~Blink;
	}

	// setzen aller Attribute
	void setAttr(CgaAttr attr)
	{
		setForeground(attr.getForeground());
		setBackground(attr.getBackground());
		setBlinkState(attr.getBlinkState());
	}

	// ermitteln der Schriftfarbe
	Color getForeground()
	{
		return Color(this->x & ForegroundColor);
	}

	// ermitteln der Hintergrundfarbe
	Color getBackground()
	{
		return Color((this->x & BackgroundColor) >> 4);
	}

	// ermitteln ob Blink-Flag gesetzt ist
	bool getBlinkState()
	{
		return (bool) (x >> 7);
	}

private:

	char x;
};

#endif
