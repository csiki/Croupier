#include "stdafx.h"
#include "Log.h"

/** Returns the begin and end const_iterator of the events list.
*/
void Log::getEventsIterators(list<Event*>::const_iterator& begin, list<Event*>::const_iterator& end) const
{
	begin = events.cbegin();
	end = events.cend();
}

/** Push an event to the log.
*/
void Log::log(Event* e)
{
	events.push_back(e);
}