#include "io/PrintStream.h"

PrintStream::PrintStream(OutputChannel* chan)
	: channel(*chan)
{

}

PrintStream::PrintStream(OutputChannel& chan)
	: channel(chan)
{

}

void PrintStream::print(const char* str)
{

}

void PrintStream::print(char ch)
{

}

void PrintStream::println(const char *str)
{

}

void PrintStream::println()
{

}

void PrintStream::print(int x, int base)
{

}

void PrintStream::print(unsigned int x, int base)
{

}

void PrintStream::print(void *p)
{

}
