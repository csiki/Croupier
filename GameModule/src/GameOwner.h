#if !defined(_GAMEOWNER_H)
#define _GAMEOWNER_H

#include "Bot.h"
#include "Log.h"
#include "BotManager.h"

/**	Initialises the participants of the game; runs it; saves the results.
*/
class GameOwner
{
private:
	const char* logPath;
	const char* rulzPath;
	const char* botsDataPath;
	const char* resultsPath;
	int numOfBots;
	Bot** bots;
	BotManager** botManagers;
	Log* log;
	Rulz* rulz;
	int gameState; // 1 - game initialised, 2 - game started, 3 - game ended, 4 - results saved, else 0 (GameOwner just constructed)
public:

	GameOwner(int numOfBots,
		const char* logPath, const char* rulzPath,
		const char* botsDataPath, const char* resultsPath)
	{
		this->logPath = logPath;
		this->rulzPath = rulzPath;
		this->botsDataPath = botsDataPath;
		this->resultsPath = resultsPath;

		// not set numOfBots, but allocate space for bots
		// set numOfBots, when loading bots
		this->bots = new Bot*[numOfBots];
		this->botManagers = new BotManager*[numOfBots];

		this->gameState = 0;
	}

	bool initialiseGame();
	int startGame();
	void saveLog();
	void saveBotResults();
	int getGameState() const;
};

#endif  //_GAMEOWNER_H
