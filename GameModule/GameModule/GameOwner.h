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
	Bot* bots;
	int numOfBots;
	Log* log;
	Rulz* rulz;
	BotManager* botManagers;
public:
	bool initialiseGame();
	int startGame();
	void saveLog();
	void saveBotResults();
};

#endif  //_GAMEOWNER_H
