#if !defined(_PROLOGBOTLOADER_H)
#define _PROLOGBOTLOADER_H

#include "BotLoader.h"

/**	Loads Prolog AIs.
*/
class PrologBotLoader : public BotLoader
{
protected:
	Process* initProcess();
public:
	Bot* loadBot(BotData* botData);
};

#endif  //_PROLOGBOTLOADER_H
