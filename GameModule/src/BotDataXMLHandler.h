#if !defined(_BOTDATAXMLHANDLER_H)
#define _BOTDATAXMLHANDLER_H

#include "BotData.h"
#include "pugixml.hpp"

/**	Loads and saves BotData in xml format.
*/
class BotDataXMLHandler
{
public:
	static BotData* loadXML(pugi::xml_node& botNode);
	static bool saveXML(BotData* botData, pugi::xml_node& botNode);
};

#endif // _BOTDATAXMLHANDLER_H