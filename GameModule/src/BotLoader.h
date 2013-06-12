#if !defined(_BOTLOADER_H)
#define _BOTLOADER_H

#include "Bot.h"
#include "Process.h"

/**	Base class for AI loader classes (factory pattern).
*/
class BotLoader
{
protected:
	virtual Process* initProcess(const char* botFilePath) = 0;
public:
	virtual Bot* loadBot(int botLang, const char* botFilePath) = 0;
};

#endif  //_BOTLOADER_H
