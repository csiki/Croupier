#if !defined(_RULZXMLHANDLER_H)
#define _RULZXMLHANDLER_H

#include "Rulz.h"

class RulzXMLHandler
{
public:
	static Rulz* loadXML(const char* xmlPath);
	static bool saveXML(Rulz* log, const char* xmlPath);
};

#endif  //_RULZXMLHANDLER_H
