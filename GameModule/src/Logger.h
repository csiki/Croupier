#if !defined(_LOGGER_H)
#define _LOGGER_H

#include "BroadcastMember.h"
#include "Entity.h"
#include "Loggable.h"
#include "Severity.h"
#include "BroadcastMessage.h"

/**	Base class for logging entities.
*/
class Logger : virtual public Entity
{
private:
	Loggable *loggable;
protected:
	void log(Severity severity, std::string msg);
public:

	Logger(int id, Loggable* loggable) : Entity(id)
	{
		this->loggable = loggable;
	}
};

#endif  //_LOGGER_H
