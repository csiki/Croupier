#if !defined(_LOGGABLE_H)
#define _LOGGABLE_H

#include "Event.h"

class Loggable
{
public:
	virtual void log(Event* e) = 0;
};

#endif  //_LOGGABLE_H
