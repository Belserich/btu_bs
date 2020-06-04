#ifndef SpuriousGate_h
#define SpuriousGate_h

#include "interrupts/Gate.h"

/*
 * 	SpuriousGate: behandelt fehlende Berechtigungen
 *
 *      Das SpuriousGate wird bei fehlender Berechtigung aktiviert,
 *      macht eine entsprechende Ausgabe und haelt die CPU an.
 */
class SpuriousGate: public Gate {
public:
	SpuriousGate();

	void handle();
};

#endif
