#if !defined(_RUBYBOTLOADER_H)
#define _RUBYBOTLOADER_H

#include "BotLoader.h"

/**	Loads Ruby AIs.
*/
class RubyBotLoader : public BotLoader
{
protected:
	Process* initProcess();
public:
	Bot* loadBot(BotData* botData);
};

#endif  //_RUBYBOTLOADER_H
