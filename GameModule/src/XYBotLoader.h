#if !defined(_XYBOTLOADER_H)
#define _XYBOTLOADER_H

#include "BotLoader.h"
#include "Process.h"

/**	(Just example of nonCpp botloader.)
*/
class XYBotLoader : public BotLoader
{
protected:
	Process* initProcess(string botFilePath);
public:
	Bot* loadBot(int botLang, string botFilePath);
};

#endif  //_XYBOTLOADER_H
