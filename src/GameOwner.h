#if !defined(_GAMEOWNER_H)
#define _GAMEOWNER_H


class GameOwner
{
public:
	bool initialiseGame();
	int startGame();
	void saveLog();
	void saveBotResults();
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
};

#endif  //_GAMEOWNER_H
