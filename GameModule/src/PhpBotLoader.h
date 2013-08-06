#if !defined(_PHPBOTLOADER_H)
#define _PHPBOTLOADER_H

#include "BotLoader.h"

/**	Loads PHP AIs.
*/
class PhpBotLoader : public BotLoader
{
protected:
	Process* initProcess();
public:
	Bot* loadBot(BotData* botData);
};

#endif  //_PHPBOTLOADER_H
