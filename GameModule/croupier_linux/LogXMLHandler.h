#if !defined(_LOGXMLHANDLER_H)
#define _LOGXMLHANDLER_H

#include "Event.h"
#include "pugixml.hpp"

/**	Loads and saves logs in xml format.
*/
class LogXMLHandler
{
public:
	static std::vector<Event> loadXML(std::string xmlPath);
	static bool saveXML(const std::vector<Event>& log, std::string xmlPath);
};

#endif  //_LOGXMLHANDLER_H
