#if !defined(_PASCALBOTLOADER_H)
#define _PASCALBOTLOADER_H

#include "BotLoader.h"

/**	Loads Pascal AIs.
*/
class PascalBotLoader : public BotLoader
{
protected:
	Process* initProcess();
public:
	Bot* loadBot(BotData* botData);
};

#endif  //_PASCALBOTLOADER_H
