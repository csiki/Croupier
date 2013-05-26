#if !defined(_EVENT_H)
#define _EVENT_H

#include "Severity.h"

class Event
{
private:
	Severity *severity;
public:
	const int loggerID;
	const char* const msg;
	const long time;
};

#endif  //_EVENT_H
