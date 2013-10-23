#include "stdafx.h"
#include "Log.h"

/** Returns the begin and end const_iterator of the events list.
*/
void Log::getEventsIterators(std::list<Event*>::const_iterator& begin, std::list<Event*>::const_iterator& end) const
{
	begin = events.cbegin();
	end = events.cend();
}

/** Gets the number of events with the given severity.
*/
int Log::getSeverityFrequency(Severity sev) const
{
	return this->severityFequency.at(sev);
}

/** Push an event to the log.
*/
void Log::log(Event* e)
{
	++this->severityFequency.at(e->severity); // increase frequency

	std::cout << e->toString() << std::endl; // TEST

	events.push_back(e);
}

