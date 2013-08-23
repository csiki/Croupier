#include "stdafx.h"
#include "Log.h"

/** Returns the begin and end const_iterator of the events list.
*/
void Log::getEventsIterators(list<Event*>::const_iterator& begin, list<Event*>::const_iterator& end) const
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

	cout << e->toString() << endl; // TODO
	events.push_back(e);
}

