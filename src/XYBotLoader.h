#if !defined(_XYBOTLOADER_H)
#define _XYBOTLOADER_H

#include "BotLoader.h"

class XYBotLoader : public BotLoader
{
public:
	Bot* loadBot(const char* const botFilePath);
private:
	Process* initProcess();
};

#endif  //_XYBOTLOADER_H
