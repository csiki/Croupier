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
#include "GameData.h"
#include "GameDataXMLHandler.h"

/**	Initialises the participants of the game; runs it; saves the results.
*/
class GameOwner
{
private:
	size_t numOfBots;
	GameData* gameData;
	Croupier* croupier;
	Hostess* hostess;
	BroadcastStation* broadcastStation;
	Bot** bots;
	BotManager** botManagers;
	Log* log;
	Rulz* rulz;
	Table* table;
	int gameState; // 1 - game initialised, 2 - game started, 3 - game ended (successfully), 4 - results saved, 5 - fatal error, else 0 (GameOwner just constructed)
	std::string errorMsg;
	std::map<BotLanguage, BotLoader*> botLoaders;

	void errorOccured(std::string msg);
	void fillBotLoaders();
public:
	GameOwner(const char* gameDataXMLPath)
	{
		this->gameData = GameDataXMLHandler::loadXML(gameDataXMLPath);
		this->numOfBots = gameData->getNumOfBots();
		this->bots = new Bot*[numOfBots];
		this->botManagers = new BotManager*[numOfBots];
		this->gameState = 0;
		this->errorMsg = "";
	}

	virtual ~GameOwner()
	{
		// delete botmanagers and bots
		for (size_t i = 0; i < this->numOfBots; ++i)
		{
			delete this->botManagers[i];
		}
		delete [] this->botManagers;
		delete [] this->bots;

		// delete others
		delete this->gameData;
		delete this->croupier;
		delete this->log;
		delete this->rulz;
		delete this->table;
		delete this->broadcastStation;
		delete this->hostess;
	}

	void initialiseGame();
	void startGame();
	void saveResults();
	int getGameState() const;
	std::string getErrorMsg() const;
};

#endif  //_GAMEOWNER_H
