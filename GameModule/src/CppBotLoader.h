#if !defined(_CPPBOTLOADER_H)
#define _CPPBOTLOADER_H

#include "BotLoader.h"

/**	Loads C++ AIs.
*/
class CppBotLoader : public BotLoader
{
protected:
	Process* initProcess(const char* botFilePath);
public:
	Bot* loadBot(int botLang, const char* botFilePath);
};

#endif  //_CPPBOTLOADER_H
