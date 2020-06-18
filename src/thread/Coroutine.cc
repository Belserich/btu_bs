#include "thread/Coroutine.h"

/*
	startup Methode
	initialisiert eine Coroutine,
	indem sie den Body und exit aufruft
*/
void Coroutine::startup(Coroutine* obj) {
	CPU::enableInterrupts();
	obj->body();
	obj->exit();
};

/*
	setup Methode
	Setzt ein SetupFrame nter den Stackpointer
*/
void Coroutine::setup(void* tos) {
	//0 also schon auf main stack 
	if (tos == 0) {
        return;
    }
	//Neues setupframe
	SetupFrame* sf = (SetupFrame*) tos;
	//Wir wollen drunterschreiben
	sf--;

    sf->edi = 0;
    sf->esi = 0;
    sf->ebx = 0;
    sf->ebp = 0;
	sf->startup = startup;
    sf->nirwana = 0;
	sf->arg = this;
	//Stackpointer retten
	this->sp = sf;

    return;
};