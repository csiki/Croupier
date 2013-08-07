#if !defined(_LOG_H)
#define _LOG_H

#include "Loggable.h"
#include "Event.h"
#include <list>

using namespace std;

/**	List of events.
*/
class Log : public Loggable
{
private:
	list<Event*> events;
	map<Severity, int> severityFequency;

public:
	Log()
	{
		// fill severity frequency (with 0s)
		for (int sevInt = Severity::FATAL; ; ++sevInt)
		{
			this->severityFequency.insert(
				pair<Severity, int>(
					static_cast<Severity>(sevInt), 0));

			if (sevInt == Severity::DEBUG)
			{
				break;
			}
		}
	}
	
	virtual ~Log()
	{
		// delete events
		for (list<Event*>::iterator it = events.begin(); it != events.end(); ++it)
		{
			delete *it;
		}
	}

	void log(Event* e);
	void getEventsIterators(list<Event*>::const_iterator& begin, list<Event*>::const_iterator& end) const;
	int getSeverityFrequency(Severity sev) const;
};

#endif  //_LOG_H
