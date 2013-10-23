#if !defined(_GAMEDATAXMLHANDLER_H)
#define _GAMEDATAXMLHANDLER_H

#include "GameData.h"
#include "BotLanguage.h"
#include "BotDataXMLHandler.h"
#include "pugixml.hpp"

/**	Loads and saves BotData in xml format.
*/
class GameDataXMLHandler
{
public:
	static GameData* loadXML(std::string xmlpath);
	static bool saveXML(GameData* gameData, std::string xmlpath);
};

#endif // _GAMEDATAXMLHANDLER_H