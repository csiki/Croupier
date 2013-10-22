#if !defined(_LOG_H)
#define _LOG_H

#include "Loggable.h"
#include "Event.h"
#include <list>

/**	List of events.
*/
class Log : public Loggable
{
private:
	std::list<Event*> events;
	std::map<Severity, int> severityFequency;

public:
	Log()
	{
		// fill severity frequency (with 0s)
		for (size_t sevInt = Severity::FATAL; ; ++sevInt)
		{
			this->severityFequency.insert(
				std::pair<Severity, int>(
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
		for (std::list<Event*>::iterator it = events.begin(); it != events.end(); ++it)
		{
			delete *it;
		}
	}

	void log(Event* e);
	void getEventsIterators(std::list<Event*>::const_iterator& begin, std::list<Event*>::const_iterator& end) const; // UNIT done
	int getSeverityFrequency(Severity sev) const;
};

#endif  //_LOG_H
