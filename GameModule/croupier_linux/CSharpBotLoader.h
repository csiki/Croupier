#if !defined(_CSHARPBOTLOADER_H)
#define _CSHARPBOTLOADER_H

#include "BotLoader.h"

/**	Loads C# AIs.
*/
class CSharpBotLoader : public BotLoader
{
protected:
	Process* initProcess();
public:
	Bot* loadBot(BotData* botData);
};

#endif  //_CSHARPBOTLOADER_H
