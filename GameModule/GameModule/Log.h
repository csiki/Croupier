#if !defined(_LOG_H)
#define _LOG_H

#include "Loggable.h"
#include "Event.h"

class Log : public Loggable
{
private:
	Event events;
public:
	Event* getEvent(int id);
	int getNumOfEvents();
	void log(Event* e);
};

#endif  //_LOG_H
