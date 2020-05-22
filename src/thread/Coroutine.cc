// belserich on 13.05.20

#include "thread/Coroutine.h"

//CgaChannel screen2;
//PrintStream out2(screen2);

void Coroutine::startup(Coroutine *obj)
{
	obj->body();
	obj->exit();
}

void Coroutine::setup(void* tos)
{
	if (tos == nullptr)
	{
		return;
	}

	// 0000 1111 0000 0
	//                ^tos
	// 0000 1111 0000 0
	// 		     ^frame
	// 0000 1111 1010 0
	//                ^tos
	//           ^sp

	this->sp = (void*) (((Frame*) tos) - 1); // -1 zieht vom Adresswert sizeof(Frame) ab; this->sp zeigt auf den tos - sizeof(Frame)
	Frame* frame = ((Frame*) tos) - 1;
	*frame = Frame(this);

//	out2.print(this->sp);
//	out2.println();
//	out2.print("Bytes Kontrollblock: ");
//	out2.print(sizeof(Frame), 10);
//	out2.println();
//
//	out2.print("Kontrollblock: ");
//	for (int i = 0; i != (sizeof(Frame) / 4); i++)
//	{
//		out2.print((void*) *(((unsigned*)this->sp) + i));
//		out2.print(" ");
//	}
//	out2.println();
}
