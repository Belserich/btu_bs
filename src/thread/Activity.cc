// belserich on 13.05.20

#include "thread/Activity.h"

Activity::Activity(void *tos) : Coroutine(tos)
{
	wakeup();
}

Activity::Activity()
{
	wakeup();
}

Activity::~Activity()
{

}

void Activity::sleep()
{

}

void Activity::wakeup()
{

}

void Activity::yield()
{

}

void Activity::exit()
{

}

void Activity::join()
{

}

