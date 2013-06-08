#if !defined(_EVENT_H)
#define _EVENT_H

#include "Severity.h"

/**	One unit of a log.
*/
class Event
{
public:
	const Severity severity;
	const int loggerID;
	const char* const msg;
	const long time;
};

#endif  //_EVENT_H
