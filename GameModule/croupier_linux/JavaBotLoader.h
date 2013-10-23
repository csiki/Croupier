#if !defined(_JAVABOTLOADER_H)
#define _JAVABOTLOADER_H

#include "BotLoader.h"

/**	Loads Java AIs.
*/
class JavaBotLoader : public BotLoader
{
protected:
	Process* initProcess();
public:
	Bot* loadBot(BotData* botData);
};

#endif  //_JAVABOTLOADER_H
