#if !defined(_RULZXMLHANDLER_H)
#define _RULZXMLHANDLER_H

#include "Rulz.h"
#include "pugixml.hpp"
#include "BotLanguage.h"

/**	Loads and saves game rules in xml format.
*/
class RulzXMLHandler
{
public:
	static Rulz* loadXML(std::string xmlPath);
	static bool saveXML(Rulz* rules, std::string xmlPath);
};

#endif  //_RULZXMLHANDLER_H
