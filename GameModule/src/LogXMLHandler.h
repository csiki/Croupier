#if !defined(_LOGXMLHANDLER_H)
#define _LOGXMLHANDLER_H

#include "Log.h"

/**	Loads and saves logs in xml format.
*/
class LogXMLHandler
{
public:
	static Log* loadXML(const char* xmlPath);
	static bool saveXML(Log* log, const char* xmlPath);
};

#endif  //_LOGXMLHANDLER_H
