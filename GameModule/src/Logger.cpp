#include "stdafx.h"
#include "Logger.h"
#include "Severity.h"
#include "BroadcastMessage.h"

/** Sends the message (event) to the log.
*/
void Logger::log(Severity severity, std::string msg) const
{
	time_t timer = time(0);

	loggable->log(new Event(severity, getID(), msg));
}
