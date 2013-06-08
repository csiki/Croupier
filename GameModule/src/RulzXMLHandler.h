#if !defined(_RULZXMLHANDLER_H)
#define _RULZXMLHANDLER_H

#include "Rulz.h"

/**	Loads and saves game rules in xml format.
*/
class RulzXMLHandler
{
public:
	static Rulz* loadXML(const char* xmlPath);
	static bool saveXML(Rulz* rules, const char* xmlPath);
};

#endif  //_RULZXMLHANDLER_H
