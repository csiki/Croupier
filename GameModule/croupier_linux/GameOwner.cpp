#include "stdafx.h"
#include "GameOwner.h"
#include "Card.h"

// TEST
#include "TestBot.h"
#include "TestBotAggressive.h"
#include "TestBotShy.h"
#include "AliveBot.h"
#include <iostream>
#include <exception>

/** Saves the error message.
*/
void GameOwner::errorOccured(std::string msg)
{
	this->gameState = 5; // error state
	this->errorMsg = msg;
    std::cerr << msg << std::endl;
}

/** Fills the botLoaders map with BotLanguage and BotLoader* pairs.
*/
void GameOwner::fillBotLoaders()
{
	// CPP
	this->botLoaders.insert(
		std::pair<BotLanguage, BotLoader*>( BotLanguage::CPP, new CppBotLoader() ));

	// ALIVE
	this->botLoaders.insert(
		std::pair<BotLanguage, BotLoader*>( BotLanguage::ALIVE, new AliveBotLoader() ));

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
	std::string rulzPath = _RULZ_RELATIVE_PATH_;
	rulzPath += this->gameData->rulzFileName;
	this->rulz = RulzXMLHandler::loadXML(rulzPath);

	if (this->rulz == nullptr)
	{
		this->errorOccured("Cannot load Rulz instance from xml!");
		return;
	}

	this->broadcastStation = new BroadcastStation;
	this->table = new Table(this->numOfBots);
	this->hostess = new Hostess(this->numOfBots, this->table, this->rulz, this->broadcastStation);
	this->croupier = new Croupier(this->numOfBots, this->broadcastStation, this->rulz, this->table);

	// load bots
	for (size_t i = 0; i < this->numOfBots; ++i)
	{
		// get bot data from game data
		BotData* botdata = this->gameData->getBotData(i);

		// create bot knowledge handler (if neccessery)
		BotKnowledgeHandler* bkHandler = nullptr;
		if (rulz->isBotKnowledgeUseAllowed())
		{
			bkHandler = new BotKnowledgeHandler(botdata->playerID, botdata->numOfKnowledgeTables, botdata->knowledgeTables);
		}

		// load bot manager
		this->botManagers[i] = new BotManager(
			bkHandler, this->hostess, this->table,
			this->rulz, this->broadcastStation, botdata->playerID, botdata->id,
			this->rulz->getStartingChips(), botdata->credit - this->rulz->getStartingChips(), i);

		// set botdata communicator
		botdata->communicator = this->botManagers[i];

		// load bot
		try
		{
			this->bots[i] = this->botLoaders.at(botdata->lang)->loadBot(botdata);
		}
		catch(BotLoaderException& e)
		{
			std::string msg = e.what();
			msg += '\n';
			msg += e.whatError();
			this->errorOccured(msg);
			return;
		}

		// connect bot to communicator
		this->botManagers[i]->monitor(this->bots[i]);

		// sit bot to table
		this->table->sit(this->botManagers[i]);

		// add BotHandler to croupier
		this->croupier->provideBotHandler(i, this->botManagers[i]);
	}

	// fill hostess' bot data (from table)
	this->hostess->fillBotsData();

	this->gameState = 1;
}

/** Trigger the component(s), to start the game.
 *	@return if the game was valid or not (if there was not any events with fatal or error severity).
*/
void GameOwner::startGame()
{
	this->croupier->letsPoker();
	this->gameState = 3;
}

/** Save the results of the game.
*/
void GameOwner::saveResults()
{
	// save bot knowledge tables (BotKnowledgeHandler destructor does it)
	// save log
	std::string logPath = _LOG_RELATIVE_PATH_;
	logPath += this->gameData->logFileName;
	LogXMLHandler::saveXML(Logger::GetEvents(), logPath);

	// save bot credits and the round when they fell out
	Results* results = new Results(this->numOfBots);

	for (size_t i = 0; i < this->numOfBots; ++i)
	{
		results->addResult(
			this->botManagers[i]->getBotID(), this->botManagers[i]->getID(),
			this->botManagers[i]->getReservedCredit() + this->botManagers[i]->getChips(),
			this->botManagers[i]->getKickedAtRound());
	}

	std::string resultsPath = _RESULTS_RELATIVE_PATH_;
	resultsPath += this->gameData->resultsFileName;
	ResultsXMLHandler::saveXML(results, resultsPath);

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
std::string GameOwner::getErrorMsg() const
{
	return this->errorMsg;
}
