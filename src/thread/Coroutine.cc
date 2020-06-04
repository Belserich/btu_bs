// belserich on 13.05.20

#include "thread/Coroutine.h"

void Coroutine::startup(Coroutine *obj)
{
	CPU::enableInterrupts();
	obj->body();
	obj->exit();
}

void Coroutine::setup(void* tos)
{
	if (tos == nullptr) // wird nur fuer anton, also diejenige Coroutine ausgefuehrt, die auf dem Hauptstack liegt
	{
		return; // kein setup fuer anton
	}

	// 0000 1111 0000 0
	//                ^tos
	// 0000 1111 0000 0
	// 		     ^frame
	// 0000 1111 1010 0
	//                ^tos
	//           ^sp

	this->sp = (void*) (((ControlBlock*) tos) - 1); // -1 zieht vom Adresswert sizeof(Frame) ab; this->sp zeigt auf (tos - sizeof(Frame))
	ControlBlock* frame = ((ControlBlock*) tos) - 1; // die gleiche Rechnung wie ueber dieser Zeile
	*frame = ControlBlock(this);
}
