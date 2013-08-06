#if !defined(_GAMEOWNER_H)
#define _GAMEOWNER_H

#include "Bot.h"
#include "Log.h"
#include "LogXMLHandler.h"
#include "RulzXMLHandler.h"
#include "Croupier.h"
#include "BotDataXMLHandler.h"
#include "BotData.h"
#include "BotLoader.h"
#include "CppBotLoader.h"
#include "BotManager.h"

/**	Initialises the participants of the game; runs it; saves the results.
*/
class GameOwner
{
private:
	const char* logPath;
	const char* rulzPath;
	const char* resultsPath;
	int numOfBots;
	int* playersID;
	Croupier* croupier;
	Hostess* hostess;
	BroadcastStation* broadcastStation;
	Bot** bots;
	BotManager** botManagers;
	Log* log;
	Rulz* rulz;
	Table* table;
	int gameState; // 1 - game initialised, 2 - game started, 3 - game ended, 4 - results saved, 5 - fatal error, else 0 (GameOwner just constructed)
	string errorMsg;
	map<BotLanguage, BotLoader*> botLoaders;

	void errorOccured(string msg);
	void fillBotLoaders();
public:
	GameOwner(int numOfBots, int* playersID,
		const char* logPath, const char* rulzPath,
		const char* resultsPath)
	{
		// set attributes
		this->playersID = playersID;
		this->logPath = logPath;
		this->rulzPath = rulzPath;
		this->resultsPath = resultsPath;
		this->numOfBots = numOfBots;
		this->bots = new Bot*[numOfBots];
		this->botManagers = new BotManager*[numOfBots];
		this->gameState = 0;

		// fill botloader
		this->fillBotLoaders();
	}

	virtual ~GameOwner()
	{
		// delete path strings
		delete this->logPath;
		delete this->rulzPath;
		delete this->resultsPath;

		// delete botmanagers and bots
		for (int i = 0; i < this->numOfBots; ++i)
		{
			delete this->botManagers[i];
			delete this->bots[i];
		}
		delete [] this->botManagers;
		delete [] this->bots;

		// delete log
		delete this->log;

		// delete rulz
		delete this->rulz;
	}

	bool initialiseGame();
	int startGame();
	void saveResults();
	int getGameState() const;
	string getErrorMsg() const;
};

#endif  //_GAMEOWNER_H
