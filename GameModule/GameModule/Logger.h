#if !defined(_LOGGER_H)
#define _LOGGER_H

#include "BroadcastMember.h"
#include "Entity.h"
#include "Loggable.h"
#include "Severity.h"
#include "BroadcastMessage.h"

class Logger : public Entity
{
private:
	Loggable *loggable;
protected:
	void log(Severity severity, char* msg);
};

#endif  //_LOGGER_H
