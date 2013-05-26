#if !defined(_BOTLOADER_H)
#define _BOTLOADER_H

#include "Bot.h"

class BotLoader
{
public:
	virtual Bot* loadBot(const char* botFilePath) = 0;
protected:
	const int botLang;
};

#endif  //_BOTLOADER_H
