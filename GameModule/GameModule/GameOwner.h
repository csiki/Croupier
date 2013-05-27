#if !defined(_GAMEOWNER_H)
#define _GAMEOWNER_H

#include "Bot.h"
#include "Log.h"
#include "BotManager.h"

class GameOwner
{
private:
	char* logPath;
	char* rulzPath;
	char* botsDataPath;
	char* resultsPath;
	int numOfBots;
	Bot* bots;
	BotManager* botManagers;
	Log* log;
	Rulz* rulz;
public:
	bool initialiseGame();
	int startGame();
	void saveLog();
	void saveBotResults();
};

#endif  //_GAMEOWNER_H
