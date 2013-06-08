#include "stdafx.h"
#include "BotManager.h"
#include "Comment.h"
#include "KnowledgeDataType.h"
#include "BroadcastMessage.h"

/// broadcast
void BotManager::receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data) {}
/// botinfo
int BotManager::getID() const
{
	return 0;
}

std::string BotManager::getName() const
{
	return 0;
}

int BotManager::getLang() const
{
	return 0;
}
/// botcommunicator
int BotManager::getReservedCredit() const
{
	return 0;
}
int BotManager::getNumOfRebuys() const
{
	return 0;
}
std::string BotManager::getBotName(int botID) const
{
	return 0;
}
bool BotManager::isBotDealer(int botID) const
{
	return 0;
}
int BotManager::getBotChips(int botID) const
{
	return 0;
}
int BotManager::getBotPot(int botID) const
{
	return 0;
}
Emotion BotManager::getBotEmotion(int botID) const
{
	return Emotion::HIDDEN;
}
bool BotManager::isBotHandRevealed(int botID) const
{
	return 0;
}
int BotManager::getBotLang(int botID) const
{
	return 0;
}
bool BotManager::isBotInGame(int botID) const
{
	return 0;
}
bool BotManager::isBotInRound(bool botID) const
{
	return 0;
}
Card BotManager::lookAtBotHand(int botID, int cardIndex) const
{
	return Card::getNullCard();
}
bool BotManager::allin()
{
	return 0;
}
bool BotManager::call() 
{
	return 0;
}
bool BotManager::canAllin() const
{
	return 0;
}
bool BotManager::canCall() const
{
	return 0;
}
bool BotManager::canCheck() const
{
	return 0;
}
bool BotManager::canFold() const
{
	return 0;
}
bool BotManager::canRaise(int raiseAmount) const
{
	return 0;
}
bool BotManager::check()
{
	return 0;
}
bool BotManager::fold()
{
	return 0;
}
bool BotManager::raise(int raiseAmount)
{
	return 0;
}
bool BotManager::canRebuy(int rebuyAmount) const
{
	return 0;
}
bool BotManager::rebuy(int rebuyAmount)
{
	return 0;
}
void BotManager::talk(Comment comment)
{
}
int BotManager::getNumOfBots(bool onlyInGame, bool onlyInRound) const
{
	return 0;
}
int BotManager::getBotIDByIndex(int index) const
{
	return 0;
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
/// bothandler
void BotManager::step() {}
void BotManager::quit() {}
bool BotManager::rebuyOrLeave()
{
	return 0;
}
