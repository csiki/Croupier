#if !defined(_CPPBOTLOADER_H)
#define _CPPBOTLOADER_H

#include "BotLoader.h"

class CppBotLoader : public BotLoader
{
public:
	Bot* loadBot(const char* const botFilePath);
};

#endif  //_CPPBOTLOADER_H
