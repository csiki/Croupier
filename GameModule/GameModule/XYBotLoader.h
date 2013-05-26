#if !defined(_XYBOTLOADER_H)
#define _XYBOTLOADER_H

#include "BotLoader.h"
#include "Process.h"

class XYBotLoader : public BotLoader
{
public:
	Bot* loadBot(const char* botFilePath);
private:
	Process* initProcess();
};

#endif  //_XYBOTLOADER_H
