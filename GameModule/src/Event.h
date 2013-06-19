#if !defined(_EVENT_H)
#define _EVENT_H

#include "Severity.h"

/**	One unit of a log.
*/
class Event
{
private:
	long getTime() const;
public:
	const Severity severity;
	const int loggerID;
	const std::string msg;
	const long time;
	Event(Severity severity, int loggerID, std::string msg); 
};

#endif  //_EVENT_H
