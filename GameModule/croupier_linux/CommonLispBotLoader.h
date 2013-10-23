#if !defined(_COMMONLISPBOTLOADER_H)
#define _COMMONLISPBOTLOADER_H

#include "BotLoader.h"

/**	Loads CommonLisp AIs.
*/
class CommonLispBotLoader : public BotLoader
{
protected:
	Process* initProcess();
public:
	Bot* loadBot(BotData* botData);
};

#endif  //_COMMONLISPBOTLOADER_H
