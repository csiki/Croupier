#if !defined(_EVENT_H)
#define _EVENT_H

#include "Severity.h"

/**	One unit of a log.
 *	Message formula: <action> [<parameter1>,<parameter2>,...]
*/
class Event
{
public:
	const Severity severity;
	const int loggerID;
	const std::string msg;

	Event(Severity severity, int loggerID, std::string msg) :
		severity(severity), loggerID(loggerID), msg(msg)
	{}
	
};

#endif  //_EVENT_H
