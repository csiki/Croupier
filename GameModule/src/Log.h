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
public:
	void log(Event* e);
	void getEventsIterators(list<Event*>::const_iterator& begin, list<Event*>::const_iterator& end) const;
};

#endif  //_LOG_H
