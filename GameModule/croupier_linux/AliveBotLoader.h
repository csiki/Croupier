#if !defined(_ALIVEBOTLOADER_H)
#define _ALIVEBOTLOADER_H

#include "BotLoader.h"

/**	Loads Humans.
*/
class AliveBotLoader : public BotLoader
{
protected:
	Process* initProcess();
public:
	Bot* loadBot(BotData* botData);
};

#endif  //_ALIVEBOTLOADER_H