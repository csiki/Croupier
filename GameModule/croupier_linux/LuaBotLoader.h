#if !defined(_LUABOTLOADER_H)
#define _LUABOTLOADER_H

#include "BotLoader.h"

/**	Loads Lua AIs.
*/
class LuaBotLoader : public BotLoader
{
protected:
	Process* initProcess();
public:
	Bot* loadBot(BotData* botData);
};

#endif  //_LUABOTLOADER_H
