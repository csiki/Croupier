#if !defined(_BOTDATAXMLHANDLER_H)
#define _BOTDATAXMLHANDLER_H

#include "BotData.h"
#include "pugixml.hpp"

/**	Loads and saves BotData in xml format.
*/
class BotDataXMLHandler
{
public:
	static BotData* loadXML(std::string xmlpath);
	static bool saveXML(BotData* botData, std::string xmlpath);
};

#endif // _BOTDATAXMLHANDLER_H