#if !defined(_SCHEMEBOTLOADER_H)
#define _SCHEMEBOTLOADER_H

#include "BotLoader.h"

/**	Loads Scheme AIs.
*/
class SchemeBotLoader : public BotLoader
{
protected:
	Process* initProcess();
public:
	Bot* loadBot(BotData* botData);
};

#endif  //_SCHEMEBOTLOADER_H
