#if !defined(_BOTLOADER_H)
#define _BOTLOADER_H

#include "Bot.h"

class BotLoader
{
protected:
	const int botLang;
public:
	virtual Bot* loadBot(const char* botFilePath) = 0;
};

#endif  //_BOTLOADER_H
