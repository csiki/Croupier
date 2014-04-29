#include "stdafx.h"
#include "BotManager.h"

// broadcast
/** Inherited method to handle incoming broadcast messages.
*/
void BotManager::receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data)
{
	this->talkToken = true; // bot can comment when receiving a message

	try
	{
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
			case QUIT:
				this->bot->leftGame(data[0]);
				break;
			case RAISED:
				this->bot->raised(data[0], data[1]);
				break;
			case REBUYOCCURRED:
				this->bot->rebuyOccurred(data[0], data[1]);
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
				if (this->inGame)
				{
					this->inRound = true;
				}
				this->bot->roundStarted(data[0]);
				break;
			case ROUNDWINNERS:
				// copying data, can't let bots delete it
				int *dataCopy;
				dataCopy = new int[dataSize];
				for (size_t i = 0; i < dataSize; ++i)
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
				// do nothing
		}
	}
	catch (BotTimeExceededException& e)
	{
		Logger::Log(this, Severity::ERROR, "timeExceeded ", this->getID(), ',', this->bot->getID(), ',', e.whatMethod());
		this->quit();
	}
	catch (std::exception& e)
	{
		Logger::Log(this, Severity::ERROR, "exceptionThrown ", this->getID(), ',', this->bot->getID(), ',', e.what());
		this->quit();
	}
	catch (...)
	{
		Logger::Log(this, Severity::ERROR, "exceptionThrown ", this->getID(), ',', this->bot->getID());
	    this->quit();
	}

	this->talkToken = false;
}

// botinfo
/** Returns id of the player.
*/
int BotManager::getBotID() const
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
BotLanguage BotManager::getLang() const
{
	return this->bot->getLang();
}

// botcommunicator
/** Returns reserved credit of the user.
*/
int BotManager::getReservedCredit() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

  Logger::Log(this, Severity::VERBOSE, "getReservedCredit");

  return this->reservedCredit;
}

/** Returns number of rebuys already done by the managed AI (in current game).
*/
int BotManager::getNumOfRebuys() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

  Logger::Log(this, Severity::VERBOSE, "getNumOfRebuys");

  return this->numOfRebuys;
}

/** Returns a specified AI's name.
*/
std::string BotManager::getBotName(int botID) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

  Logger::Log(this, Severity::VERBOSE, "getBotName ", botID);

    const BotInfo* bot = this->hostess->getBotByID(botID);
    if(bot != nullptr)
        return bot->getName();
    else
        return "";
}

/** Returns if a specified AI is a dealer.
*/
bool BotManager::isBotDealer(int botID) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

  Logger::Log(this, Severity::VERBOSE, "isBotDealer ", botID);

    const BotInfo* bot = this->hostess->getBotByID(botID);
    if(bot != nullptr)
        return bot->isDealer();
    else
        return false;
}

/** Returns a specified AI's chips at hand.
*/
int BotManager::getBotChips(int botID) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

  Logger::Log(this, Severity::VERBOSE, "getBotChips ", botID);

    const BotInfo* bot = this->hostess->getBotByID(botID);
    if(bot != nullptr)
        return bot->getChips();
    else
        return -1;
}

/** Returns a specified AI's pot in game.
*/
int BotManager::getBotPot(int botID) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

  Logger::Log(this, Severity::VERBOSE, "getBotPot ", botID);

    const BotInfo* bot = this->hostess->getBotByID(botID);
    if(bot != nullptr)
        return bot->getPot();
    else
        return -1;
}

/** Returns a specified AI's name.
*/
Emotion BotManager::getBotEmotion(int botID) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

  Logger::Log(this, Severity::VERBOSE, "getBotEmotion ", botID);

    const BotInfo* bot = this->hostess->getBotByID(botID);
    if(bot != nullptr)
        return bot->getEmotion();
    else
        return Emotion::DRUNKEN;
}

/** Returns if a specified AI's hand is revealed.
*/
bool BotManager::isBotHandRevealed(int botID) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

  Logger::Log(this, Severity::VERBOSE, "isBotHandRevealed ", botID);

    const BotInfo* bot = this->hostess->getBotByID(botID);
    if(bot != nullptr)
        return bot->isHandRevealed();
    else
        return false;
}

/** Returns a specified AI's languge it is written in.
*/
BotLanguage BotManager::getBotLang(int botID) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

  Logger::Log(this, Severity::VERBOSE, "getBotLang ", botID);

    const BotInfo* bot = this->hostess->getBotByID(botID);
    if(bot != nullptr)
        return bot->getLang();
    else
        return BotLanguage::CPP;
}

/** Returns if a specified AI's is in game (haven't fallen out or quited).
*/
bool BotManager::isBotInGame(int botID) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

  Logger::Log(this, Severity::VERBOSE, "isBotInGame ", botID);

    const BotInfo* bot = this->hostess->getBotByID(botID);
    if(bot != nullptr)
        return bot->isInGame();
    else
        return false;
}

/** Returns if a specified AI's is in round (in game and haven't folded).
*/
bool BotManager::isBotInRound(bool botID) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

  Logger::Log(this, Severity::VERBOSE, "isBotInRound ", botID);

    const BotInfo* bot = this->hostess->getBotByID(botID);
    if(bot != nullptr)
        return bot->isInRound();
    else
        return false;
}

/** Returns a specified AI's card in hand if revealed, else NullCard.
*/
Card BotManager::lookAtBotHand(int botID, int cardIndex) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

  Logger::Log(this, Severity::VERBOSE, "lookAtBotHand ", botID);

    const BotInfo* bot = this->hostess->getBotByID(botID);
    if(bot != nullptr)
        return bot->lookAtHand(cardIndex);
    else
        return Card::getNullCard();
}

/** Returns if a specified AI can talk.
*/
bool BotManager::canTalk() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

  Logger::Log(this, Severity::VERBOSE, "canTalk");

	return this->talkToken;
}

/** Returns if a specified AI can step (check or fold or ...).
*/
bool BotManager::canStep() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

  Logger::Log(this, Severity::VERBOSE, "canStep");

  return this->stepToken;
}

/** Returns if allin is a possible movement.
*/
bool BotManager::canAllin() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

  Logger::Log(this, Severity::VERBOSE, "canAllin");

	BettingSystem bs = this->rules->getBettingSystem();
	int callAmountOfPlayer = this->hostess->getCallAmount() - this->pot;

	return this->stepToken
		&& this->chips > 0
		&& (
			bs == BettingSystem::NOLIMIT
			|| (bs == BettingSystem::FIXLIMIT && this->chips <= callAmountOfPlayer) // if fixlimit, allin is only possible, when chips <= call
			|| (bs == BettingSystem::POTLIMIT && this->chips <= this->table->getPot() + callAmountOfPlayer) // if potlimit, it's max is the whole pot + player's call
			); // TODO
}

/** Returns if call is a possible movement.
*/
bool BotManager::canCall() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

  Logger::Log(this, Severity::VERBOSE, "canCall");

	int callAmountOfPlayer = this->hostess->getCallAmount() - this->pot;

	return this->stepToken
		&& callAmountOfPlayer > 0 // to have anything to call (else should check)
		&& this->chips >= callAmountOfPlayer; // to have enough chips (else allin)
}

/** Returns if check is a possible movement.
*/
bool BotManager::canCheck() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "canCheck");

	return this->stepToken && (this->hostess->getCallAmount() - this->pot) == 0;
}

/** Returns if fold is a possible movement.
*/
bool BotManager::canFold() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "canFold");

	return this->stepToken;
}

/** Returns if raising raiseAmount is a possible movement.
*/
bool BotManager::canRaise(int raiseAmount) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "canRaise ", raiseAmount);

	int minRaise = this->hostess->getMinRaise();
	int callAmountOfPlayer = this->hostess->getCallAmount() - this->pot;
	BettingSystem bs = this->rules->getBettingSystem();

	return this->stepToken
		&& (this->rules->getBettingSystem() != BettingSystem::FIXLIMIT || (this->rules->getBettingSystem() == BettingSystem::FIXLIMIT && this->numOfRaises < 4))
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
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::INFORMATION, "allin");

	if (this->canAllin())
	{
		int allinAmount = this->chips;
		this->pot += this->chips;
		this->chips = 0;

		this->stepToken = false;

		// broadcast allin
		int* msgdata = new int[2];
		msgdata[0] = this->getID();
		msgdata[1] = allinAmount;
		this->broadcast(BroadcastMessage::ALLINED, 2, msgdata);
		delete [] msgdata;
		return true;
	}
	return false;
}

/** Bot signals call.
*/
bool BotManager::call()
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::INFORMATION, "call");

	if (this->canCall())
	{
		int callAmount = this->hostess->getCallAmount() - this->pot;
		this->chips -= callAmount;
		this->pot += callAmount;

		this->stepToken = false;

		// broadcast call
		int* msgdata = new int[2];
		msgdata[0] = this->getID();
		msgdata[1] = callAmount;
		this->broadcast(BroadcastMessage::CALLED, 2, msgdata);
		delete [] msgdata;
		return true;
	}
	return false;
}

/** Bot signals check.
*/
bool BotManager::check()
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::INFORMATION, "check");

	if (this->canCheck())
	{
		this->stepToken = false;

		// broadcast check
		int msgdata = this->getID();
		this->broadcast(BroadcastMessage::CHECKED, 1, &msgdata);

		return true;
	}

	return false;
}

/** Bot signals fold.
*/
bool BotManager::fold()
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::INFORMATION, "fold");

	if (this->canFold())
	{
		this->inRound = false;
		this->stepToken = false;

		// broadcast fold
		int msgdata = this->getID();
		this->broadcast(BroadcastMessage::FOLDED, 1, &msgdata);
		return true;
	}
	return false;
}

/** Bot signals raise.
*/
bool BotManager::raise(int raiseAmount)
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::INFORMATION, "raise ", raiseAmount);

	if (this->canRaise(raiseAmount))
	{
		int chipsToMove = (this->hostess->getCallAmount() - this->pot) + raiseAmount;
		this->chips -= chipsToMove;
		this->pot += chipsToMove;

		this->stepToken = false;

		// broadcast raise
		int* msgdata = new int[2];
		msgdata[0] = this->getID();
		msgdata[1] = raiseAmount;
		this->broadcast(BroadcastMessage::RAISED, 2, msgdata);
		delete [] msgdata;

		// increase number of raises
		++this->numOfRaises;
		return true;
	}
	return false;
}

/** Returns if a rebuy is possible.
*/
bool BotManager::canRebuy(int rebuyAmount) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "canRebuy ", rebuyAmount);

	return this->numOfRebuys < this->rules->getNumOfRebuysAllowed()
		&& this->hostess->getCurrentRound() <= this->rules->getRebuyDeadline()
		&& rebuyAmount <= this->reservedCredit;
}

/** Bot signals rebuy.
*/
bool BotManager::rebuy(int rebuyAmount)
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::INFORMATION, "rebuy ", rebuyAmount);

	if (this->canRebuy(rebuyAmount))
	{
		this->reservedCredit -= rebuyAmount;
		this->chips += rebuyAmount;
		++this->numOfRebuys;

		// broadcast rebuy
		int* msgdata = new int[2];
		msgdata[0] = this->getID();
		msgdata[1] = rebuyAmount;
		this->broadcast(BroadcastMessage::REBUYOCCURRED, 2, msgdata);
		delete [] msgdata;
		return true;
	}
	return false;
}

/** Bot signals talk.
*/
bool BotManager::talk(Comment comment)
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::INFORMATION, "talk ", comment);

	if (this->talkToken)
	{
		// broadcast comment
		int* msgdata = new int[2];
		msgdata[0] = this->getID();
		msgdata[1] = comment;
		this->broadcast(BroadcastMessage::LISTEN, 2, msgdata);
		delete [] msgdata;
		return true;
	}
	return false;
}

/** Bot signals to leave the game.
 *	Bot::leave() won't be called this way; bot instantly and permanently gets out of the game.
*/
void BotManager::quit()
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::INFORMATION, "quit");

	this->inRound = false;
	this->inGame = false;
	this->kickedAtRound = this->hostess->getCurrentRound();
	this->unsubscribe();

	// broadcast left game
	int msgdata = this->getID();
	this->broadcast(BroadcastMessage::QUIT, 1, &msgdata);
}

/** Returns the number of AIs.
*/
int BotManager::getNumOfBots(bool onlyInGame, bool onlyInRound) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getNumOfBots ", onlyInGame, ',', onlyInRound);

	return this->hostess->getNumOfBots(onlyInGame, onlyInRound);
}

/** Returns AI's id by indexing (index: from 0 to n-1 same order at table; n: number of bots).
*/
int BotManager::getBotIDByIndex(int index) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getBotIDByIndex ", index);

    const BotInfo* bot = this->table->getBotByIndex(index);
    if(bot != nullptr)
        return bot->getID();
    else
        return -1;
}

/** Returns AI's index by id (index: from 0 to n-1 same order at table; n: number of bots).
*/
int BotManager::getBotIndexByID(int botID) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getBotIndexByID ", botID);

	return this->hostess->getBotIDByIndex(botID);
}

/** Returns nth AI's id to the right (at table).
*/
int BotManager::getBotIDToTheRight(int nth, bool onlyInGame, bool onlyInRound) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getBotIDToTheRight ", nth, ',', onlyInGame, ',', onlyInRound);

	return this->hostess->getBotIDToTheRight(this->nthAtTable, nth, onlyInGame, onlyInRound);
}

/** Returns nth AI's id to the left (at table).
*/
int BotManager::getBotIDToTheLeft(int nth, bool onlyInGame, bool onlyInRound) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getBotIDToTheLeft ", nth, ',', onlyInGame, ',', onlyInRound);

	return this->hostess->getBotIDToTheLeft(this->nthAtTable, nth, onlyInGame, onlyInRound);
}

/** Returns the amount of pot that should be put in by the player if it would call.
*/
int BotManager::getCallAmount() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

  Logger::Log(this, Severity::VERBOSE, "getCallAmount");

	return this->hostess->getCallAmount() - this->pot;
}

/** Returns the minimum of raise that can be put in above callAmount.
*/
int BotManager::getMinRaise() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getMinRaise");

	return this->hostess->getMinRaise();
}

/** Returns big blind at specific round (present/future/past).
*/
int BotManager::getBigBlindAtRound(int round) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	if (round == -1) // -1 def arg
	{
		round = this->hostess->getCurrentRound();
	}

	Logger::Log(this, Severity::VERBOSE, "getBigBlindAtRound ", round);

	return this->hostess->getBigBlindAtRound(round);
}

/** Returns the round when the croupier shifts blind at shiftDeadlineIndex time.
*/
int BotManager::getBlindShiftDeadline(int shiftDeadlineIndex) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getBlindShiftDeadline ", shiftDeadlineIndex);

	return this->rules->getBlindShiftDeadline(shiftDeadlineIndex);
}

/** Returns the number of the round when croupier shifts blind next time.
*/
int BotManager::getNextBlindShiftDeadline() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getNextBlindShiftDeadline");

	return this->hostess->getNextBlindShiftDeadline();
}

/** Returns small blind at specific round (present/future/past).
*/
int BotManager::getSmallBlindAtRound(int round) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	if (round == -1)
	{
		round = this->hostess->getCurrentRound();
	}

	Logger::Log(this, Severity::VERBOSE, "getSmallBlindAtRound ", round);

	return this->hostess->getSmallBlindAtRound(round);
}

/** Returns the number of the current round.
*/
int BotManager::getCurrentRound() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getCurrentRound");

	return this->hostess->getCurrentRound();
}

/** Returns the number of cards on table (flop = 3 etc.).
*/
int BotManager::getTableNumOfCards() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getCurrentRound");

	return this->table->getNumOfCards();
}

/** Returns a card from table.
*/
Card BotManager::getTableCard(int cardIndex) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getTableCard ", cardIndex);

	return this->table->getCard(cardIndex);
}

/** Returns the amount of pot on table.
*/
int BotManager::getPotSum() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getPotSum");

	return this->table->getPot();
}

/** Returns a big blind.
*/
int BotManager::getBigBlind(int blindIndex) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getBigBlind ", blindIndex);

	return this->rules->getBigBlind(blindIndex);
}

/** Returns deadline till rebuy is allowed.
*/
int BotManager::getRebuyDeadline() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getRebuyDeadline");

	return this->rules->getRebuyDeadline();
}

/** Returns small blind.
*/
int BotManager::getSmallBlind(int blindIndex) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getSmallBlind ", blindIndex);

	return this->rules->getSmallBlind(blindIndex);
}

/** Returns allowed time for AI calculation with specific language (0 = cpp etc.).
*/
int BotManager::getAllowedBotCalcTime() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getAllowedBotCalcTime");

	return this->rules->getAllowedBotCalcTime(this->bot->getLang());
}

/** Returns the amount of chips with all the bot starts.
*/
int BotManager::getStartingChips() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getStartingChips");

	return this->rules->getStartingChips();
}

/** Returns number of blind shifts / or the number of blinds.
*/
int BotManager::getNumOfBlinds() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getNumOfBlinds");

	return this->rules->getNumOfBlinds();
}

/** Returns number of rebuys allowed.
*/
int BotManager::getNumOfRebuysAllowed() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getNumOfRebuysAllowed");

	return this->rules->getNumOfRebuysAllowed();
}

/** Returns if talk is allowed.
*/
bool BotManager::isTalkAllowed() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "isTalkAllowed");

	return this->rules->isTalkAllowed();
}

/** Returns if expressing emotions are allowed.
*/
bool BotManager::isEmotionAllowed() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "isEmotionAllowed");

	return this->rules->isEmotionAllowed();
}

/** Returns if using botknowledge is permitted.
*/
bool BotManager::isBotKnowledgeUseAllowed() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "isBotKnowledgeUseAllowed");

	return this->rules->isBotKnowledgeUseAllowed();
}

/** Returns if the table with the given id is loaded.
*/
bool BotManager::isTableLoaded(int tableID) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "isTableLoaded");

	if (this->rules->isBotKnowledgeUseAllowed())
	{
		return this->bkHandler->isTableLoaded(tableID);
	}
	return false;
}

/** Returns the highest rank of the bot's two cards combined with the cards on table.
*/
HandRank BotManager::getHandRank() const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getHandRank");

	if (this->hand.size() == 2) // has cards
	{
		return this->hostess->getBotHandRank(this->hand);
	}
	return HandRank::None;
}

/** Adds a row to the specified table.
*/
int BotManager::addKnowledgeTableRow(int tableID)
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "addKnowledgeTableRow ", tableID);

	if (this->rules->isBotKnowledgeUseAllowed())
	{
	  return this->bkHandler->addTableRow(tableID);
	}
	return 0;
}

/** Creates a knowledge table.
*/
int BotManager::createKnowledgeTable(int numOfCols, std::list<KnowledgeDataType> colTypes)
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	// log
  std::ostringstream os;
  os << "createKnowledgeTable " << numOfCols;
	for (const auto& it : colTypes)
	{
		os << ',' << it;
	}
	Logger::Log(this, Severity::VERBOSE, os.str());

	if (this->rules->isBotKnowledgeUseAllowed() && numOfCols == colTypes.size())
	{
		// create and fill colTypes array
		KnowledgeDataType* tmpColTypes = new KnowledgeDataType[numOfCols];

		int i = 0;
		for (std::list<KnowledgeDataType>::iterator it = colTypes.begin(); it != colTypes.end(); ++it)
		{
			tmpColTypes[i++] = *it;
		}

    return this->bkHandler->createTable(numOfCols, tmpColTypes);
	}
	return 0;
}

/** Returns data type of specific column.
*/
KnowledgeDataType BotManager::getKnowledgeTableDataType(int tableID, int col) const
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getKnowledgeTableDataType ", tableID, ',', col);

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
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getKnowledgeTableData(int) ", tableID, ',', row, ',', col);

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
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getKnowledgeTableData(bool) ", tableID, ',', row, ',', col);

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
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getKnowledgeTableData(char) ", tableID, ',', row, ',', col);

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
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getKnowledgeTableData(string) ", tableID, ',', row, ',', col);

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
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getKnowledgeTableData(float) ", tableID, ',', row, ',', col);

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
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "setKnowledgeTableData(int) ", tableID, ',', row, ',', col, ',', val);

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
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "setKnowledgeTableData(bool) ", tableID, ',', row, ',', col, ',', val);

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
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "setKnowledgeTableData(char) ", tableID, ',', row, ',', col, ',', val);

	if (this->rules->isBotKnowledgeUseAllowed())
	{
	  return this->bkHandler->setTableData(val, tableID, row, col);
	}
	return false;
}

/** Sets data at specific cell in a knowledge table (const char* ~ std::string).
*/
bool BotManager::setKnowledgeTableData(const char* val, int tableID, int row, int col)
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

  Logger::Log(this, Severity::VERBOSE, "setKnowledgeTableData(str) ", tableID, ',', row, ',', col, ',', val);

  return this->setKnowledgeTableData(std::string(val), tableID, row, col);
}

/** Sets data at specific cell in a knowledge table (std::string).
*/
bool BotManager::setKnowledgeTableData(std::string val, int tableID, int row, int col)
{
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

  Logger::Log(this, Severity::VERBOSE, "setKnowledgeTableData(str) ", tableID, ',', row, ',', col, ',', val);

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
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "setKnowledgeTableData(float) ", tableID, ',', row, ',', col, ',', val);

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
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getKnowledgeTableNumOfCols ", tableID);

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
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "getKnowledgeTableNumOfRows ", tableID);

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
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "removeKnowledgeTable ", tableID);

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
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);

	Logger::Log(this, Severity::VERBOSE, "removeKnowledgeTableRow ", tableID, ',', row);

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
	this->stepToken = true;

	// call step (calls a wrapper, possibly TimerBotProxy)
	try
	{
		this->bot->step();
	}
	catch (BotTimeExceededException& e)
	{
		Logger::Log(this, Severity::ERROR, "timeExceeded ", this->getID(), ',', this->bot->getID(), ',', e.whatMethod());
		this->fold();
		this->quit();
	}
	catch (std::exception& e)
	{
		Logger::Log(this, Severity::ERROR, "exceptionThrown ", this->getID(), ',', this->bot->getID(), ',', e.what());
        this->fold();
		this->quit();
	}
	catch (...)
	{
   		Logger::Log(this, Severity::ERROR, "exceptionThrown ", this->getID(), ',', this->bot->getID());
        this->fold();
	    this->quit();
	}

	if (this->stepToken) // no step taken
	{
		Logger::Log(this, Severity::WARNING, "noStepTaken");
		this->fold();
	}

	this->stepToken = false;
}

/** Croupier signals that AI should quit permanently.
*/
void BotManager::leave()
{
	this->inGame = false;
	this->inRound = false;
	this->kickedAtRound = this->hostess->getCurrentRound();
	this->unsubscribe();

	try
	{
		this->bot->leave();
	}
	catch (BotTimeExceededException& e)
	{
	  Logger::Log(this, Severity::ERROR, "timeExceeded ", this->getID(), ',', this->bot->getID(), ',', e.whatMethod());
	}
	catch (std::exception& e)
	{
  		Logger::Log(this, Severity::ERROR, "exceptionThrown ", this->getID(), ',', this->bot->getID(), ',', e.what());
	}
	catch (...)
	{
		Logger::Log(this, Severity::ERROR, "exceptionThrown ", this->getID(), ',', this->bot->getID());
	}

	// broadcast left game
	int msgdata = this->getID();
	this->broadcast(BroadcastMessage::LEFTGAME, 1, &msgdata);
}

/** Croupier signals that AI should rebuy or else leave the game.
*/
void BotManager::rebuyOrLeave()
{
	if (this->canRebuy(1))
	{
		try
		{
			this->bot->rebuyOrLeave();
		}
		catch (BotTimeExceededException& e)
		{
			Logger::Log(this, Severity::ERROR, "timeExceeded ", this->getID(), ',', this->bot->getID(), ',', e.whatMethod());
			this->quit();
		}
		catch (std::exception& e)
        {
			Logger::Log(this, Severity::ERROR, "exceptionThrown ", this->getID(), ',', this->bot->getID(), ',', e.what());
            this->quit();
        }
        catch (...)
        {
      		Logger::Log(this, Severity::ERROR, "exceptionThrown ", this->getID(), ',', this->bot->getID());
			this->quit();
        }
	}
}

// own
/** Returns the number of round when the bot left (or kicked out of) the game.
*/
int BotManager::getKickedAtRound() const
{
	return this->kickedAtRound;
}

/** Sets the bot to monitor and wrap in a timer proxy.
 *	Can be called only once, else memory leak occurs!
*/
void BotManager::monitor(Bot* bot)
{
	this->bot = new TimerBotProxy(bot, this->rules->getAllowedBotCalcTime(bot->getLang()),
	    this->manage_thread_mutex);
}
