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
#include "AliveBotLoader.h"
#include "BotManager.h"
#include "Results.h"
#include "ResultsXMLHandler.h"

/**	Initialises the participants of the game; runs it; saves the results.
*/
class GameOwner
{
private:
	string logPath;
	string rulzPath;
	string resultsPath;
	int numOfBots;
	int* playersID;
	Croupier* croupier;
	Hostess* hostess;
	BroadcastStation* broadcastStation;
	Bot** bots;
	BotManager** botManagers;
	BotData** botsData;
	Log* log;
	Rulz* rulz;
	Table* table;
	int gameState; // 1 - game initialised, 2 - game started, 3 - game ended (successfully), 4 - results saved, 5 - fatal error, else 0 (GameOwner just constructed)
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
		this->botsData = new BotData*[numOfBots];
		this->gameState = 0;
		this->errorMsg = "";
	}

	virtual ~GameOwner()
	{
		// delete botmanagers and bots
		for (int i = 0; i < this->numOfBots; ++i)
		{
			delete this->botManagers[i];
			delete this->bots[i];
			delete this->botsData[i];
		}
		delete [] this->botManagers;
		delete [] this->bots;
		delete [] this->botsData;

		// delete others
		delete this->croupier;
		delete this->log;
		delete this->rulz;
		delete this->table;
		delete this->broadcastStation;
		delete this->hostess;
		delete [] this->playersID;
	}

	void initialiseGame();
	void startGame();
	void saveResults();
	int getGameState() const;
	string getErrorMsg() const;
};

#endif  //_GAMEOWNER_H
