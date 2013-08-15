#include "stdafx.h"
#include "GameOwner.h"
#include "TestBot.h"
#include "Card.h"

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

		// load bot TODO
		/*this->bots[i] = this->botLoaders.at(botData->lang)->loadBot(botData);
		if (this->bots[i] == nullptr)
		{
			string msg = "Cannot load Bot instance from BotData! Bot id: ";
			msg += to_string(botData->id);
			this->errorOccured(msg);
			return false;
		}*/
		this->bots[i] = new TestBot(30, "jaja", BotLanguage::CPP);

		// create bot knowledge handler (if neccessery)
		BotKnowledgeHandler* bkHandler = nullptr;
		if (rulz->isBotKnowledgeUseAllowed())
		{
			bkHandler = new BotKnowledgeHandler(botData);
		}

		// load bot manager
		this->botManagers[i] = new BotManager(
			this->bots[i], bkHandler, this->hostess, this->table,
			this->rulz, this->broadcastStation, log, this->playersID[i],
			this->rulz->getStartingChips(), botData->credit - this->rulz->getStartingChips(), i);

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

/** Trigger the component(s), to start the game.
 *	@return if the game was valid or not (if there was not any events with fatal or error severity).
*/
bool GameOwner::startGame()
{
	this->croupier->letsPoker();

	if (this->log->getSeverityFrequency(Severity::FATAL) > 0
		|| this->log->getSeverityFrequency(Severity::ERROR) > 0)
	{
		this->errorOccured("Event with FATAL or ERROR severity occured during the game!");
		return false;
	}

	return true;
}

/** Save the results of the game.
*/
void GameOwner::saveResults()
{
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

	// save log
	LogXMLHandler::saveXML(this->log, this->logPath);

	// save bot knowledge tables (BotKnowledgeHandler destructor)
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

/** For testing.
*/
void GameOwner::test()
{
	this->initialiseGame();

	cout << this->errorMsg;
	
	this->botManagers[0]->addDealerButton();
	
	int d = 1;
	this->croupier->broadcast(BroadcastMessage::ROUNDSTARTED, 1, &d);
	
	const Card** bestHand1 = new const Card*[5];
	const Card** bestHand2 = new const Card*[5];
	const Card** cards1 = new const Card*[7];
	const Card** cards2 = new const Card*[7];

	// table
	Card** tablecards = new Card*[5];
	tablecards[0] = new Card(Card::Suit::CLUBS, Card::Rank::QUEEN);
	tablecards[1] = new Card(Card::Suit::DIAMONDS, Card::Rank::QUEEN);
	tablecards[2] = new Card(Card::Suit::DIAMONDS, Card::Rank::KING);
	tablecards[3] = new Card(Card::Suit::CLUBS, Card::Rank::THREE);
	tablecards[4] = new Card(Card::Suit::DIAMONDS, Card::Rank::DEUCE);

	for (int i = 0; i < 5; ++i)
	{
		cards1[i] = tablecards[i];
		cards2[i] = tablecards[i];
	}

	cards1[5] = new const Card(Card::Suit::HEARTS, Card::Rank::QUEEN);
	cards1[6] = new const Card(Card::Suit::SPADES, Card::Rank::DEUCE);

	cards2[5] = new const Card(Card::Suit::SPADES, Card::Rank::QUEEN);
	cards2[6] = new const Card(Card::Suit::HEARTS, Card::Rank::THREE);
	
	cout << "best1" << endl;
	cout << HandEvaluator::evalHand(cards1, bestHand1) << endl;
	for (int i = 0; i < 5; ++i)
	{
		cout << bestHand1[i]->toString() << endl;
	}

	cout << "best2" << endl;
	cout << HandEvaluator::evalHand(cards2, bestHand2) << endl;
	for (int i = 0; i < 5; ++i)
	{
		cout << bestHand2[i]->toString() << endl;
	}

	cout << "comp 1-2: " << HandEvaluator::handComparator(HandRank::FullHouses, bestHand1, bestHand2) << endl;


	delete [] bestHand1;
	delete [] bestHand2;
	delete [] cards1;
	delete [] cards2;
	delete [] tablecards;
}