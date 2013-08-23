#include "stdafx.h"
#include "Event.h"

string Event::toString() const
{
	// formula: <severity> #<logger-id> <msg>
	string result;

	// severity
	switch(this->severity)
	{
	case Severity::FATAL:
		result = "FATAL #";
		break;
	case Severity::ERROR:
		result = "ERROR #";
		break;
	case Severity::WARNING:
		result = "WARNING #";
		break;
	case Severity::NOTIFICATION:
		result = "NOTIFICATION #";
		break;
	case Severity::INFORMATION:
		result = "INFORMATION #";
		break;
	case Severity::VERBOSE:
		result = "VERBOSE #";
		break;
	case Severity::DEBUG:
		result = "DEBUG #";
		break;
	default:
		result = "??? #";
		break;
	}

	// logger id
	result += to_string(this->loggerID);
	result += " ";

	// msg
	result += this->msg;

	return result;
}