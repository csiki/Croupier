#if !defined(_CPPBOTLOADER_H)
#define _CPPBOTLOADER_H

#include "BotLoader.h"
#include "BotLoaderException.h"
#include "dlfcn_just_header.h"

/**	Loads C++ AIs.
*/
class CppBotLoader : public BotLoader
{
private:
	std::map<Bot*, destroy_bot*> destroyers;

protected:
	Process* initProcess(); // UNIT

public:
	Bot* loadBot(BotData* botData); // UNIT

	~CppBotLoader()
	{
		for (std::map<Bot*, destroy_bot*>::iterator it = destroyers.begin(); it != destroyers.end(); ++it)
		{
			(it->second)(it->first);
		}
	}
};

#endif  //_CPPBOTLOADER_H
