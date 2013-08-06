#if !defined(_CBOTLOADER_H)
#define _CBOTLOADER_H

#include "BotLoader.h"

/**	Loads C AIs.
*/
class CBotLoader : public BotLoader
{
protected:
	Process* initProcess();
public:
	Bot* loadBot(BotData* botData);
};

#endif  //_CBOTLOADER_H
