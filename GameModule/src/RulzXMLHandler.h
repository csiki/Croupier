#if !defined(_RULZXMLHANDLER_H)
#define _RULZXMLHANDLER_H

#include "Rulz.h"

/**	Loads and saves game rules in xml format.
*/
class RulzXMLHandler
{
public:
	static Rulz* loadXML(string xmlPath);
	static bool saveXML(Rulz* rules, string xmlPath);
};

#endif  //_RULZXMLHANDLER_H
