#include "stdafx.h"
#include "BotManager.h"

// broadcast
/** Inherited method to handle incoming broadcast messages.
*/
void BotManager::receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data)
{
	// TODO idõ mérése !
	int *dataCopy;
	this->talkToken = true; // bot can comment when receiving a message

	switch (msg)
	{
		case ALLINED:
			this->bot->allined(data[0], data[1]);
			break;
		case BLINDSRAISED:
			this->bot->blindsRaised(data[0], data[1]);
			break;
		case CALLED:
			this->bot->called(data[0], data[1]);
			break;
		case CHECKED:
			this->bot->checked(data[0]);
			break;
		case FLOP:
			this->bot->flop();
			break;
		case FOLDED:
			this->bot->folded(data[0]);
			break;
		case GAMEWINNER:
			this->bot->gameWinner(data[0]);
			break;
		case LEFTGAME:
			this->bot->leftGame(data[0]);
			break;
		case LISTEN:
			this->bot->listen(data[0], (Comment)data[1]);
			break;
		case PREFLOP:
			this->bot->preflop();
			break;
		case RAISED:
			this->bot->raised(data[0], data[1]);
			break;
		case REBUYDEADLINEREACHED:
			this->bot->rebuyDeadlineReached();
			break;
		case RIVER:
			this->bot->river();
			break;
		case ROUNDENDED:
			this->bot->roundEnded();
			break;
		case ROUNDSTARTED:
			this->bot->roundStarted(data[0]);
			break;
		case ROUNDWINNERS:
			// copying data, can't let bots delete it
			dataCopy = new int[dataSize];
			for (int i = 0; i < dataSize; ++i)
				dataCopy[i] = data[i];
 			this->bot->roundWinners(dataSize, dataCopy);
			delete [] dataCopy;
			break;
		case SHOWDOWN:
			this->bot->showdown();
			break;
		case TURN:
			this->bot->turn();
			break;
		default: ;
			// TODO ide vmi logot a rossz mûködésre vagy exception
	}

	this->talkToken = false;
	// idõmérés vége
}

// botinfo
/** Returns id of the managed AI.
*/
int BotManager::getID() const
{
	return this->bot->getID();
}

/** Returns name of the managed AI.
*/
std::string BotManager::getName() const
{
	return this->bot->getName();
}

/** Returns language of the managed AI.
*/
int BotManager::getLang() const
{
	return this->bot->getLang();
}

// botcommunicator
/** Returns reserved credit of the user.
*/
int BotManager::getReservedCredit() const
{
	return this->reservedCredit;
}

/** Returns number of rebuys already done by the managed AI (in current game).
*/
int BotManager::getNumOfRebuys() const
{
	return this->numOfRebuys;
}

/** Returns a specified AI's name.
*/
std::string BotManager::getBotName(int botID) const
{
	return this->hostess->getBotByID(botID)->getName();
}

/** Returns if a specified AI is a dealer.
*/
bool BotManager::isBotDealer(int botID) const
{
	return this->hostess->getBotByID(botID)->isDealer();
}

/** Returns a specified AI's chips at hand.
*/
int BotManager::getBotChips(int botID) const
{
	return this->hostess->getBotByID(botID)->getChips();
}

/** Returns a specified AI's pot in game.
*/
int BotManager::getBotPot(int botID) const
{
	return this->hostess->getBotByID(botID)->getPot();
}

/** Returns a specified AI's name.
*/
Emotion BotManager::getBotEmotion(int botID) const
{
	return this->hostess->getBotByID(botID)->getEmotion();
}

/** Returns if a specified AI's hand is revealed.
*/
bool BotManager::isBotHandRevealed(int botID) const
{
	return this->hostess->getBotByID(botID)->isHandRevealed();
}

/** Returns a specified AI's languge it is written in.
*/
int BotManager::getBotLang(int botID) const
{
	return this->hostess->getBotByID(botID)->getLang();
}

/** Returns if a specified AI's is in game (haven't fallen out or quited).
*/
bool BotManager::isBotInGame(int botID) const
{
	return this->hostess->getBotByID(botID)->isInGame();
}

/** Returns if a specified AI's is in round (in game and haven't folded).
*/
bool BotManager::isBotInRound(bool botID) const
{
	return this->hostess->getBotByID(botID)->isInRound();
}

/** Returns a specified AI's card in hand if revealed, else NullCard.
*/
Card BotManager::lookAtBotHand(int botID, int cardIndex) const
{
	return this->hostess->getBotByID(botID)->lookAtHand(cardIndex);
}

/** Returns if a specified AI can talk.
*/
bool BotManager::canTalk() const
{
	return this->talkToken;
}

/** Returns if a specified AI can step (check or fold or ...).
*/
bool BotManager::canStep() const
{
	return this->stepToken;
}

/** Returns if allin is a possible movement.
*/
bool BotManager::canAllin() const
{
	BettingSystem bs = this->rules->getBettingSystem();
	int callAmountOfPlayer = this->hostess->getCallAmount() - this->pot;

	return this->stepToken
		&& this->chips > 0
		&& (
			bs == BettingSystem::NOLIMIT
			|| (bs == BettingSystem::FIXLIMIT && this->chips <= callAmountOfPlayer) // if fixlimit, allin is only possible, when chips <= call
			|| (bs == BettingSystem::POTLIMIT && this->chips <= this->table->getPot() + callAmountOfPlayer) // if potlimit, it's max is the whole pot + player's call
			);
}

/** Returns if call is a possible movement.
*/
bool BotManager::canCall() const
{
	int callAmountOfPlayer = this->hostess->getCallAmount() - this->pot;

	return this->stepToken
		&& callAmountOfPlayer > 0 // to have anything to call (else should check)
		&& this->chips >= callAmountOfPlayer; // to have enough chips (else allin)
}

/** Returns if check is a possible movement.
*/
bool BotManager::canCheck() const
{
	return this->stepToken && (this->hostess->getCallAmount() - this->pot) == 0;
}

/** Returns if fold is a possible movement.
*/
bool BotManager::canFold() const
{
	return this->stepToken;
}

/** Returns if raising raiseAmount is a possible movement.
*/
bool BotManager::canRaise(int raiseAmount) const
{
	int minRaise = this->hostess->getMinRaise();
	int callAmountOfPlayer = this->hostess->getCallAmount() - this->pot;
	BettingSystem bs = this->rules->getBettingSystem();

	return this->stepToken
		&& this->hostess->getNumberOfRaisesLeft() > 0
		&& this->chips >= callAmountOfPlayer + raiseAmount
		&& (
			(bs == BettingSystem::NOLIMIT && raiseAmount >= minRaise) // raise >= bigblind (minRaise)
			|| (bs == BettingSystem::FIXLIMIT && raiseAmount == minRaise) // raise == bigblind or 2*bigblind (minRaise)
			|| (bs == BettingSystem::POTLIMIT && raiseAmount >= minRaise
				&& raiseAmount <= this->table->getPot() + callAmountOfPlayer) // bigblind <= last raise amount (minRaise) <= raise <= potsize+call
			);
}

/** Bot signals allin.
*/
bool BotManager::allin()
{
	if (this->canAllin())
	{
		this->pot += this->chips;
		this->chips = 0;

		this->stepToken = false;
		return true;
	}

	return false;
}

/** Bot signals call.
*/
bool BotManager::call() 
{
	if (this->canCall())
	{
		int callAmount = this->hostess->getCallAmount() - this->pot;
		this->chips -= callAmount;
		this->pot += callAmount;

		this->stepToken = false;
		return true;
	}

	return false;
}

/** Bot signals check.
*/
bool BotManager::check()
{
	if (this->canCheck())
	{
		this->stepToken = false;
		return true;
	}

	return false;
}

/** Bot signals fold.
*/
bool BotManager::fold()
{
	if (this->canFold())
	{
		this->inRound = false;

		this->stepToken = false;
		return true;
	}

	return false;
}

/** Bot signals raise.
*/
bool BotManager::raise(int raiseAmount)
{
	if (this->canRaise(raiseAmount))
	{
		int chipsToMove = (this->hostess->getCallAmount() - this->pot) + raiseAmount;
		this->chips -= chipsToMove;
		this->pot += chipsToMove;

		this->stepToken = false;
		return true;
	}

	return false;
}

/** Returns if a rebuy is possible.
*/
bool BotManager::canRebuy(int rebuyAmount) const
{
	return this->numOfRebuys < this->rules->getNumOfRebuysAllowed()
		&& this->hostess->getCurrentRound() <= this->rules->getRebuyDeadline()
		&& rebuyAmount <= this->reservedCredit;
}

/** Bot signals rebuy.
*/
bool BotManager::rebuy(int rebuyAmount)
{
	if (this->canRebuy(rebuyAmount))
	{
		this->reservedCredit -= rebuyAmount;
		this->chips += rebuyAmount;

		return true;
	}

	return false;
}

/** Bot signals talk.
*/
void BotManager::talk(Comment comment)
{
	if (this->talkToken)
	{
		int* p = new int;
		*p = comment;
		this->broadcast(BroadcastMessage::LISTEN, 1, p);
		delete p;
	}
}

/** Returns the number of AIs.
*/
int BotManager::getNumOfBots(bool onlyInGame, bool onlyInRound) const
{
	return this->hostess->getNumOfBots(onlyInGame, onlyInRound);
}

/** Returns AI's id by indexing (index: from 0 to n-1 same order at table; n: number of bots).
*/
int BotManager::getBotIDByIndex(int index) const
{
	return this->table->getBotByIndex(index)->getID();
}

/** Returns AI's index by id (index: from 0 to n-1 same order at table; n: number of bots).
*/
int BotManager::getBotIndexByID(int botID) const
{
	return this->hostess->getBotIDByIndex(botID);
}

/** Returns nth AI's id to the right (at table).
*/
int BotManager::getBotIDToTheRight(int nth, bool onlyInGame, bool onlyInRound) const
{
	return this->hostess->getBotIDToTheRight(this->nthAtTable, nth, onlyInGame, onlyInRound);
}

/** Returns nth AI's id to the left (at table).
*/
int BotManager::getBotIDToTheLeft(int nth, bool onlyInGame, bool onlyInRound) const
{
	return this->hostess->getBotIDToTheLeft(this->nthAtTable, nth, onlyInGame, onlyInRound);
}

/** Returns the amount of pot that should be put in by the player if it calls.
*/
int BotManager::getCallAmount() const
{
	return this->hostess->getCallAmount() - this->pot;
}

/** Returns the minimum of raise that can be put in above callAmount.
*/
int BotManager::getMinRaise() const
{
	return this->hostess->getMinRaise();
}

/** Returns big blind at specific round (present/future/past).
*/
int BotManager::getBigBlindAtRound(int round) const
{
	if (round == -1)
	{
		round = this->hostess->getCurrentRound();
	}

	return this->hostess->getBigBlindAtRound(round);
}

/** Returns the round when the croupier shifts blind at shiftDeadlineIndex time.
*/
int BotManager::getBlindShiftDeadline(int shiftDeadlineIndex) const
{
	return this->rules->getBlindShiftDeadline(shiftDeadlineIndex);
}

/** Returns the number of the round when croupier shifts blind next time.
*/
int BotManager::getNextBlindShiftDeadline() const
{
	return this->hostess->getNextBlindShiftDeadline();
}

/** Returns small blind at specific round (present/future/past).
*/
int BotManager::getSmallBlindAtRound(int round) const
{
	if (round == -1)
	{
		round = this->hostess->getCurrentRound();
	}

	return this->hostess->getSmallBlindAtRound(round);
}

/** Returns the number of the current round.
*/
int BotManager::getCurrentRound() const
{
	return this->hostess->getCurrentRound();
}

/** Returns the number of cards on table (flop = 3 etc.).
*/
int BotManager::getTableNumOfCards() const
{
	return this->table->getNumOfCards();
}

/** Returns a card from table.
*/
Card BotManager::getTableCard(int cardIndex) const
{
	return *this->table->getCard(cardIndex);
}

/** Returns the amount of pot on table.
*/
int BotManager::getPotSum() const
{
	return this->table->getPot();
}

/** Returns a big blind.
*/
int BotManager::getBigBlind(int blindIndex) const
{
	return this->rules->getBigBlind(blindIndex);
}

/** Returns deadline till rebuy is allowed.
*/
int BotManager::getRebuyDeadline() const
{
	return this->rules->getRebuyDeadline();
}

/** Returns max number of raises in a round.
*/
int BotManager::getMaxNumOfRaises() const
{
	return this->rules->getMaxNumOfRaises();
}

/** Returns small blind.
*/
int BotManager::getSmallBlind(int blindIndex) const
{
	return this->rules->getSmallBlind(blindIndex);
}

/** Returns allowed time for AI calculation with specific language (0 = cpp etc.).
*/
long BotManager::getAllowedBotCalcTime(int langID) const
{
	return this->rules->getAllowedBotCalcTime(langID);
}

/** Returns the amount of chips with all the bot starts.
*/
int BotManager::getStartingChips() const
{
	return this->rules->getStartingChips();
}

/** Returns number of blind shifts / or the number of blinds.
*/
int BotManager::genNumOfBlinds() const
{
	return this->rules->getNumOfBlinds();
}

/** Returns number of rebuys allowed.
*/
int BotManager::getNumOfRebuysAllowed() const
{
	return this->rules->getNumOfRebuysAllowed();
}

/** Returns if talk is allowed.
*/
bool BotManager::isTalkAllowed() const
{
	return this->rules->isTalkAllowed();
}

/** Returns if expressing emotions are allowed.
*/
bool BotManager::isEmotionAllowed() const
{
	return this->rules->isEmotionAllowed();
}

/** Returns if using botknowledge is permitted.
*/
bool BotManager::isBotKnowledgeUseAllowed() const
{
	return this->rules->isBotKnowledgeUseAllowed();
}

/** Adds a row to the specified table.
*/
int BotManager::addKnowledgeTableRow(int tableID)
{
	if (this->rules->isBotKnowledgeUseAllowed())
	{
		return this->bkHandler->addTableRow(tableID);
	}

	return 0;
}

/** Creates a knowledge table.
*/
int BotManager::createKnowledgeTable(int numOfCols, KnowledgeDataType* colTypes)
{
	if (this->rules->isBotKnowledgeUseAllowed())
	{
		return this->createKnowledgeTable(numOfCols, colTypes);
	}

	return 0;
}

/** Returns data type of specific column.
*/
KnowledgeDataType BotManager::getKnowledgeTableDataType(int tableID, int col) const
{
	if (this->rules->isBotKnowledgeUseAllowed())
	{
		return this->bkHandler->getTableColumnType(tableID, col);
	}

	return KnowledgeDataType::NONE;
}

/** Gets knowledge table data (int).
*/
bool BotManager::getKnowledgeTableData(int& val, int tableID, int row, int col) const
{
	if (this->rules->isBotKnowledgeUseAllowed())
	{
		return this->bkHandler->getTableData(val, tableID, row, col);
	}

	return false;
}

/** Gets knowledge table data (bool).
*/
bool BotManager::getKnowledgeTableData(bool& val, int tableID, int row, int col) const
{
	if (this->rules->isBotKnowledgeUseAllowed())
	{
		return this->bkHandler->getTableData(val, tableID, row, col);
	}

	return false;
}

/** Gets knowledge table data (char).
*/
bool BotManager::getKnowledgeTableData(char& val, int tableID, int row, int col) const
{
	if (this->rules->isBotKnowledgeUseAllowed())
	{
		return this->bkHandler->getTableData(val, tableID, row, col);
	}

	return false;
}

/** Gets knowledge table data (string).
*/
bool BotManager::getKnowledgeTableData(std::string& val, int tableID, int row, int col) const
{
	if (this->rules->isBotKnowledgeUseAllowed())
	{
		return this->bkHandler->getTableData(val, tableID, row, col);
	}

	return false;
}

/** Gets knowledge table data (float).
*/
bool BotManager::getKnowledgeTableData(float& val, int tableID, int row, int col) const
{
	if (this->rules->isBotKnowledgeUseAllowed())
	{
		return this->bkHandler->getTableData(val, tableID, row, col);
	}

	return false;
}

/** Sets data at specific cell in a knowledge table (int).
*/
bool BotManager::setKnowledgeTableData(int val, int tableID, int row, int col)
{
	if (this->rules->isBotKnowledgeUseAllowed())
	{
		return this->bkHandler->setTableData(val, tableID, row, col);
	}

	return false;
}

/** Sets data at specific cell in a knowledge table (bool).
*/
bool BotManager::setKnowledgeTableData(bool val, int tableID, int row, int col)
{
	if (this->rules->isBotKnowledgeUseAllowed())
	{
		return this->bkHandler->setTableData(val, tableID, row, col);
	}

	return false;
}

/** Sets data at specific cell in a knowledge table (char).
*/
bool BotManager::setKnowledgeTableData(char val, int tableID, int row, int col)
{
	if (this->rules->isBotKnowledgeUseAllowed())
	{
		return this->bkHandler->setTableData(val, tableID, row, col);
	}

	return false;
}

/** Sets data at specific cell in a knowledge table (std::string).
*/
bool BotManager::setKnowledgeTableData(std::string val, int tableID, int row, int col)
{
	if (this->rules->isBotKnowledgeUseAllowed())
	{
		return this->bkHandler->setTableData(val, tableID, row, col);
	}

	return false;
}

/** Sets data at specific cell in a knowledge table (float).
*/
bool BotManager::setKnowledgeTableData(float val, int tableID, int row, int col)
{
	if (this->rules->isBotKnowledgeUseAllowed())
	{
		return this->bkHandler->setTableData(val, tableID, row, col);
	}

	return false;
}

/** Returns number of columns of specific knowledge table.
*/
int BotManager::getKnowledgeTableNumOfCols(int tableID) const
{
	if (this->rules->isBotKnowledgeUseAllowed())
	{
		return this->bkHandler->getTableNumOfCols(tableID);
	}

	return 0;
}

/** Returns number of rows of specific knowledge table.
*/
int BotManager::getKnowledgeTableNumOfRows(int tableID) const
{
	if (this->rules->isBotKnowledgeUseAllowed())
	{
		return this->bkHandler->getTableNumOfRows(tableID);
	}

	return 0;
}

/** Removes a specific knowledge table permanently.
*/
bool BotManager::removeKnowledgeTable(int tableID)
{
	if (this->rules->isBotKnowledgeUseAllowed())
	{
		return this->bkHandler->removeTable(tableID);
	}

	return false;
}

/** Removes a specific row of a knowledge table permanently.
*/
bool BotManager::removeKnowledgeTableRow(int tableID, int row)
{
	if (this->rules->isBotKnowledgeUseAllowed())
	{
		return this->bkHandler->removeTableRow(tableID, row);
	}

	return false;
}

// bothandler
/** Croupier signals that AI should make a move.
*/
void BotManager::step()
{
	// TODO idõt mérni !!!
	this->bot->step();

	if (this->stepToken)
	{
		this->fold(); // default move if no made
	}
}

/** Croupier signals that AI should quit permanently.
*/
void BotManager::leave()
{
	// TODO idõt mérni !!!
	this->bot->leave();
}

/** Croupier signals that AI should rebuy or else leave the game.
*/
bool BotManager::rebuyOrLeave()
{
	// TODO idõt mérni !!!
	this->bot->rebuyOrLeave();

	if (this->chips > 0)
	{
		return true;
	}

	return false;
}
