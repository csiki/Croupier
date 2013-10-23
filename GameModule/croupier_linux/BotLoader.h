#if !defined(_BOTLOADER_H)
#define _BOTLOADER_H

#include "Bot.h"
#include "Process.h"
#include "BotData.h"

/**	Base class for AI loader classes (factory pattern).
*/
class BotLoader
{
protected:
	/** Initialise a process (if needed) for the bot to run on.
	*/
	virtual Process* initProcess() = 0;

public:
	/** Loads a bot.
	*/
	virtual Bot* loadBot(BotData* botData) = 0;

	virtual ~BotLoader()
	{

	}
};

#endif  //_BOTLOADER_H
