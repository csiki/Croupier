#if !defined(_LOGGER_H)
#define _LOGGER_H

#include "BroadcastMember.h"
#include "Entity.h"
#include "Loggable.h"
#include "Severity.h"
#include "BroadcastMessage.h"

class Logger : public BroadcastMember, public Entity
{
private:
	Loggable *log;
protected:
	void log(Severity severity, char* msg);
public:
	void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* const data);
};

#endif  //_LOGGER_H
