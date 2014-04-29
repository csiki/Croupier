#if !defined(_LOGGER_H)
#define _LOGGER_H

#include "stdafx.h"
#include "Entity.h"
#include "Event.h"
#include "Severity.h"

class Logger
{
private:
  static std::vector<Event> events;

  static void toString(std::ostringstream& os) {}

  template<typename Head, typename... Tail>
  static void toString(std::ostringstream& os, Head h, Tail... t)
  {
    os << h;
    toString(os, t...);
  }

public:
	template<typename... T>
	static void Log(const Entity* entty, Severity severity, T... params)
	{
	  std::ostringstream os;
	  toString(os, params...);
	  events.push_back(Event(severity, entty->getID(), os.str()));
	}

	static const std::vector<Event>& GetEvents()
  {
	  return events;
  }
};

#endif  //_LOGGER_H
