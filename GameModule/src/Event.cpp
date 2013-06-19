#include "stdafx.h"
#include "ctime"
#include "Event.h"
#include "Severity.h"

Event::Event(Severity severity, int loggerID, std::string msg) : severity(severity), loggerID(loggerID), msg(msg), time(this->getTime()) {}

long Event::getTime() const
{
	time_t t;
	std::time(&t);
	return (long)t;
}
