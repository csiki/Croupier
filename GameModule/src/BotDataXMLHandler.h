#if !defined(_BOTDATAXMLHANDLER_H)
#define _BOTDATAXMLHANDLER_H

#include "BotData.h"
#include "pugixml.hpp"

/**	Loads and saves BotData in xml format.
*/
class BotDataXMLHandler
{
public:
	static BotData* loadXML(string xmlpath);
	static bool saveXML(BotData* botData, string xmlpath); // TODO ide plusz botnevet!!
};

#endif // _BOTDATAXMLHANDLER_H