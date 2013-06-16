#include "stdafx.h"
#include "BotManager.h"
#include "Comment.h"
#include "KnowledgeDataType.h"
#include "BroadcastMessage.h"

// broadcast
/** Inherited method to handle incoming broadcast messages.
*/
void BotManager::receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data)
{
	// TODO
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
	// TODO -> which language represents which number?
	return 0;
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
	return this->stepToken && this->chips > 0;
}

/** Returns if call is a possible movement.
*/
bool BotManager::canCall() const
{
	return this->stepToken
		&& (this->hostess->getCallAmount() - this->pot) > 0 // to have anything to call (else should check)
		&& this->chips <= (this->hostess->getCallAmount() - this->pot);
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
 *  Checks if the raiseAmount (= potToPutIn - callAmount) minimum the amount of current bigblind.
*/
bool BotManager::canRaise(int raiseAmount) const
{
	return this->stepToken && raiseAmount > this->getBigBlindAtRound();
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
		int chipsToMove = this->hostess->getCallAmount() - this->pot + raiseAmount;
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
	this->table->getBotByIndex(index)->getID();
}

int BotManager::getBotIDToTheRight(int nth, bool onlyInRound) const
{
	return 0;
}
int BotManager::getBotIDToTheLeft(int nth, bool onlyInRound) const
{
	return 0;
}
int BotManager::getCallAmount() const
{
	return 0;
}
int BotManager::getMinRaise() const
{
	return 0;
}
int BotManager::getBigBlindAtRound(int round) const
{
	return 0;
}
int BotManager::getBlindShiftDeadline(int shiftDeadlineIndex) const
{
	return 0;
}
int BotManager::getNextBlindShiftDeadline() const
{
	return 0;
}
int BotManager::getSmallBlindAtRound(int round) const
{
	return 0;
}
int BotManager::getCurrentRound() const
{
	return 0;
}

int BotManager::getTableNumOfCards() const
{
	return 0;
}
Card BotManager::getTableCard(int cardIndex) const
{
	return Card::getNullCard();
}
int BotManager::getPotSum() const
{
	return 0;
}
int BotManager::getBigBlind(int blindIndex) const
{
	return 0;
}
int BotManager::getRebuyDeadline() const
{
	return 0;
}
int BotManager::getSmallBlind(int blindIndex) const
{
	return 0;
}
long BotManager::getAllowedBotCalcTime() const
{
	return 0;
}
int BotManager::getStartingChips() const
{
	return 0;
}
int BotManager::genNumOfBlinds() const
{
	return 0;
}
int BotManager::getNumOfRebuysAllowed() const
{
	return 0;
}
bool BotManager::isTalkAllowed() const
{
	return 0;
}
bool BotManager::isBotKnowledgeUseAllowed() const
{
	return 0;
}
int BotManager::addKnowledgeTableRow(int tableID)
{
	return 0;
}
int BotManager::createKnowledgeTable(int numOfCols, KnowledgeDataType* colTypes)
{
	return 0;
}
KnowledgeDataType BotManager::getKnowledgeTableDataType(int tableID, int col) const
{
	return KnowledgeDataType::BOOL;
}
int BotManager::getKnowledgeTableDataInt(int tableID, int row, int col, bool* error) const
{
	return 0;
}
bool BotManager::getKnowledgeTableDataBool(int tableID, int row, int col, bool* error) const
{
	return 0;
}
char BotManager::getKnowledgeTableDataChar(int tableID, int row, int col, bool* error) const
{
	return 0;
}
std::string BotManager::getKnowledgeTableDataString(int tableID, int row, int col, bool* error) const
{
	return 0;
}
float BotManager::getKnowledgeTableDataFloat(int tableID, int row, int col, bool* error) const
{
	return 0;
}
int BotManager::getKnowledgeTableNumOfCols(int tableID) const
{
	return 0;
}
int BotManager::getKnowledgeTableNumOfRows(int tableID) const
{
	return 0;
}
bool BotManager::removeKnowledgeTable(int tableID)
{
	return 0;
}
bool BotManager::removeKnowledgeTableRow(int tableID, int row)
{
	return 0;
}
void BotManager::setKnowledgeTableData(int val, int tableID, int row, int col, bool* error) {}
void BotManager::setKnowledgeTableData(bool val, int tableID, int row, int col, bool* error) {}
void BotManager::setKnowledgeTableData(char val, int tableID, int row, int col, bool* error) {}
void BotManager::setKnowledgeTableData(char* val, int tableID, int row, int col, bool* error) {}
void BotManager::setKnowledgeTableData(std::string val, int tableID, int row, int col, bool* error) // same as char*
{

}
void BotManager::setKnowledgeTableData(float val, int tableID, int row, int col, bool* error) {}
// bothandler
void BotManager::step() {}
void BotManager::quit() {}
bool BotManager::rebuyOrLeave()
{
	return 0;
}
