#include "stdafx.h"
#include "Log.h"

void Log::getEventsIterators(list<Event*>::iterator& begin, list<Event*>::iterator& end) const
{
	//TODO: ezt mi a rákért nem fogadja el?
	//begin = events.begin();
	//end = events.end();
}

void Log::log(Event* e)
{
	events.push_front(e);
}