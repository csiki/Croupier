#if !defined(_PYTHONBOTLOADER_H)
#define _PYTHONBOTLOADER_H

#include "BotLoader.h"

/**	Loads Python AIs.
*/
class PythonBotLoader : public BotLoader
{
protected:
	Process* initProcess();
public:
	Bot* loadBot(BotData* botData);
};

#endif  //_PYTHONBOTLOADER_H
