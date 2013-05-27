#if !defined(_LOG_H)
#define _LOG_H

#include "Loggable.h"
#include "Event.h"
#include <list>

using namespace std;

class Log : public Loggable
{
private:
	list<Event*> events;
public:
	void getEventsIterator(list<Event*>::iterator& begin, list<Event*>::iterator& end);
	void log(Event* e);
	//Event* getEvent(int id); // not adviseable O(id)
	//int getNumOfEvents(); // no reason if getEvent() not used
};

#endif  //_LOG_H
