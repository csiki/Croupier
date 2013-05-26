#if !defined(_LOGGABLE_H)
#define _LOGGABLE_H


class Loggable
{
public:
	virtual void log(Event* e) = 0;
};

#endif  //_LOGGABLE_H
