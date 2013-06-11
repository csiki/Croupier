#if !defined(_XYBOTLOADER_H)
#define _XYBOTLOADER_H

#include "BotLoader.h"
#include "Process.h"

/**	(Just example of nonCpp botloader.)
*/
class XYBotLoader : public BotLoader
{
protected:
	Process* initProcess(const char* botFilePath);
public:
	Bot* loadBot(int botLang, const char* botFilePath);
};

#endif  //_XYBOTLOADER_H