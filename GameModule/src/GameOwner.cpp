#include "stdafx.h"
#include "GameOwner.h"
#include "TestBot.h"
#include "TestBotAggressive.h"
#include "TestBotShy.h"
#include "Card.h"

/** Saves the error message.
*/
void GameOwner::errorOccured(string msg)
{
	this->gameState = 5; // error state
	this->errorMsg = msg;
}

/** Fills the botLoaders map with BotLanguage and BotLoader* pairs.
*/
void GameOwner::fillBotLoaders()
{
	// CPP
	this->botLoaders.insert(
		pair<BotLanguage, BotLoader*>( BotLanguage::CPP, new CppBotLoader() ));
	
	// ALIVE
	this->botLoaders.insert(
		pair<BotLanguage, BotLoader*>( BotLanguage::ALIVE, new AliveBotLoader() ));

	// FILL WITH FURTHER LOADERS HERE !
}

/** Construct all the components of a game.
*/
void GameOwner::initialiseGame()
{
	// set random seed
	srand((unsigned int)time(0));

	// fill botloaders map
	this->fillBotLoaders();

	// create independent components
	this->log = new Log();
	this->rulz = RulzXMLHandler::loadXML(this->rulzPath);
	
	if (this->rulz == nullptr)
	{
		this->errorOccured("Cannot load Rulz instance from xml!");
		return;
	}

	this->broadcastStation = new BroadcastStation;
	this->table = new Table(this->numOfBots);
	this->hostess = new Hostess(this->numOfBots, this->table, this->rulz, this->broadcastStation);
	this->croupier = new Croupier(this->numOfBots, this->broadcastStation, this->log, this->rulz, this->table);

	// load bots
	for (int i = 0; i < this->numOfBots; ++i)
	{
		// load bot data
		string path = _BOT_DATA_RELATIVE_PATH_;
		path += to_string(this->playersID[i]);
		path += ".xml";

		this->botsData[i] = BotDataXMLHandler::loadXML(path);
		if (this->botsData[i] == nullptr)
		{
			string msg = "Cannot load BotData instance from xml! Player id: ";
			msg += to_string(this->playersID[i]);
			this->errorOccured(msg);
			return;
		}

		// create bot knowledge handler (if neccessery)
		BotKnowledgeHandler* bkHandler = nullptr;
		if (rulz->isBotKnowledgeUseAllowed())
		{
			bkHandler = new BotKnowledgeHandler(this->botsData[i]);
		}

		// load bot manager
		this->botManagers[i] = new BotManager(
			bkHandler, this->hostess, this->table,
			this->rulz, this->broadcastStation, log, this->playersID[i],
			this->rulz->getStartingChips(), this->botsData[i]->credit - this->rulz->getStartingChips(), i);
		
		// set botdata communicator
		this->botsData[i]->communicator = this->botManagers[i];

		// load bot TODO
		/*this->bots[i] = this->botLoaders.at(botData->lang)->loadBot(botData);
		if (this->bots[i] == nullptr)
		{
			string msg = "Cannot load Bot instance from BotData! Bot id: ";
			msg += to_string(botData->id);
			this->errorOccured(msg);
			return false;
		}*/

		// TEST
		if (i == 0)
			this->bots[i] = new TestBot(this->botManagers[i], 10, "testbot", BotLanguage::CPP);
		else if (i == 1)
			this->bots[i] = new TestBotAggressive(this->botManagers[i], 20, "aggressivebot", BotLanguage::CPP);
		else
			this->bots[i] = new TestBotShy(this->botManagers[i], 30, "shybot", BotLanguage::CPP);

		// connect bot to communicator
		this->botManagers[i]->monitor(this->bots[i]);

		// sit bot to table
		this->table->sit(this->botManagers[i]);

		// add BotHandler to croupier
		this->croupier->provideBotHandler(i, this->botManagers[i]);
	}

	// fill hostess' bot data (from table)
	this->hostess->fillBotsData();

	// delete bot loaders (not needed anyomore)
	for (map<BotLanguage, BotLoader*>::iterator it = this->botLoaders.begin(); it != this->botLoaders.end(); ++it)
	{
		delete it->second;
	}

	this->gameState = 1;
}

/** Trigger the component(s), to start the game.
 *	@return if the game was valid or not (if there was not any events with fatal or error severity).
*/
void GameOwner::startGame()
{
	this->croupier->letsPoker();

	if (this->log->getSeverityFrequency(Severity::FATAL) == 0) // TODO nem feltétlen kell, fatalokra dobálhat exceptiont is
	{
		// game ended successfully
		this->gameState = 3;
	}
}

/** Save the results of the game.
*/
void GameOwner::saveResults()
{
	// save bot knowledge tables (BotKnowledgeHandler destructor done it)
	// save log
	LogXMLHandler::saveXML(this->log, this->logPath);

	// save bot credits and the round when they fell out
	Results* results = new Results(this->numOfBots);

	for (int i = 0; i < this->numOfBots; ++i)
	{
		results->addResult(
			this->botManagers[i]->getID(),
			this->botManagers[i]->getReservedCredit() + this->botManagers[i]->getChips(),
			this->botManagers[i]->getKickedAtRound());
	}

	ResultsXMLHandler::saveXML(results, this->resultsPath);

	this->gameState = 4;
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
