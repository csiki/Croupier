#include "stdafx.h"
#include "GameOwner.h"

/** Saves the error message.
*/
void GameOwner::errorOccured(string msg)
{
	this->gameState = 5; // fatal error
	this->errorMsg = msg;
}

/** Fills the botLoaders map with BotLanguage and BotLoader* pairs.
*/
void GameOwner::fillBotLoaders()
{
	// CPP
	this->botLoaders.insert(
		pair<BotLanguage, BotLoader*>( BotLanguage::CPP, new CppBotLoader() ));
	
	// [TODO] FILL WITH FURTHER LOADERS HERE !
}

/** Construct all the components of a game.
*/
bool GameOwner::initialiseGame()
{
	// fill botloaders map
	this->fillBotLoaders();

	// create independent components
	this->log = new Log();
	this->rulz = RulzXMLHandler::loadXML(this->rulzPath);
	
	if (this->rulz == nullptr)
	{
		this->errorOccured("Cannot load Rulz instance from xml!");
		return false;
	}

	this->broadcastStation = new BroadcastStation;
	this->table = new Table(this->numOfBots);
	this->hostess = new Hostess(this->table, this->rulz, this->broadcastStation);
	this->croupier = new Croupier(this->numOfBots, this->broadcastStation, this->log, this->rulz, this->table);

	// load bots
	for (int i = 0; i < this->numOfBots; ++i)
	{
		// load bot data
		string path = _BOT_DATA_RELATIVE_PATH_;
		path += to_string(this->playersID[i]);
		path += ".xml";

		BotData* botData = BotDataXMLHandler::loadXML(path);
		if (botData == nullptr)
		{
			string msg = "Cannot load BotData instance from xml! Player id: ";
			msg += to_string(this->playersID[i]);
			this->errorOccured(msg);
			return false;
		}

		// load bot
		this->bots[i] = this->botLoaders.at(botData->lang)->loadBot(botData);
		if (this->bots[i] == nullptr)
		{
			string msg = "Cannot load Bot instance from BotData! Bot id: ";
			msg += to_string(botData->id);
			this->errorOccured(msg);
			return false;
		}

		// create bot knowledge handler (if neccessery)
		BotKnowledgeHandler* bkHandler = nullptr;
		if (rulz->isBotKnowledgeUseAllowed())
		{
			bkHandler = new BotKnowledgeHandler(botData);
		}

		// load bot manager
		this->botManagers[i] = new BotManager(
			this->bots[i], bkHandler, hostess, table,
			rulz, broadcastStation, log, this->playersID[i],
			rulz->getStartingChips(), botData->credit - rulz->getStartingChips(), i);

		// sit bot to table
		this->table->sit(this->botManagers[i]);

		// add BotHandler to croupier
		this->croupier->provideBotHandler(i, this->botManagers[i]);

		// delete bot data (not needed anymore)
		delete botData;
	}

	// fill hostess' bot data (from table)
	this->hostess->fillBotsData();

	// delete bot loaders (not needed anyomore)
	for (map<BotLanguage, BotLoader*>::iterator it = this->botLoaders.begin(); it != this->botLoaders.end(); ++it)
	{
		delete it->second;
	}

	return true;
}

/** Trigger the croupier mainly, to start the game.
*/
int GameOwner::startGame()
{
	return 0;
}

/** Save the results of the game.
*/
void GameOwner::saveResults()
{
}

/** Returns state of the game.
*/
int GameOwner::getGameState() const
{
	return this->gameState;
}

/** Gets the message of the last error occured.
*/
string GameOwner::getErrorMsg() const
{
	return this->errorMsg;
}