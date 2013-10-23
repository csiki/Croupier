#include "stdafx.h"
#include "Logger.h"
#include "Severity.h"
#include "BroadcastMessage.h"

/** Sends the message (event) to the log.
*/
void Logger::log(Severity severity, std::string msg) const
{
	if (this->logEnabled)
	{
		loggable->log(new Event(severity, getID(), msg));
	}
}

/** Enables logging.
*/
void Logger::enableLog()
{
	this->logEnabled = true;
}

/** Disables the possibility of logging.
*/
void Logger::disableLog()
{
	this->logEnabled = false;
}