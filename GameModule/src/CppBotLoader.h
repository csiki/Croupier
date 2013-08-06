#if !defined(_CPPBOTLOADER_H)
#define _CPPBOTLOADER_H

#include "BotLoader.h"

/**	Loads C++ AIs.
*/
class CppBotLoader : public BotLoader
{
protected:
	Process* initProcess();
public:
	Bot* loadBot(BotData* botData);
};

#endif  //_CPPBOTLOADER_H
