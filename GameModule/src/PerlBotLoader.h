#if !defined(_PERLBOTLOADER_H)
#define _PERLBOTLOADER_H

#include "BotLoader.h"

/**	Loads Perl AIs.
*/
class PerlBotLoader : public BotLoader
{
protected:
	Process* initProcess();
public:
	Bot* loadBot(BotData* botData);
};

#endif  //_PERLBOTLOADER_H
