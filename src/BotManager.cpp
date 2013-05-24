#include "BotManager.h"
#include "Comment.h"
#include "KnowledgeDataType.h"
#include "BroadcastMessage.h"

bool BotManager::allin()
{

}

bool BotManager::call()
{

}

bool BotManager::canAllin()
{

}

bool BotManager::canCall()
{

}

bool BotManager::canCheck()
{

}

bool BotManager::canFold()
{

}

bool BotManager::canRaise(int raiseAmount)
{

}

bool BotManager::canRebuy(int rebuyAmount)
{

}

bool BotManager::check()
{

}

bool BotManager::fold()
{

}

int BotManager::getNumOfBots(bool onlyInRound = false)
{

}

const BotInfo* const BotManager::getBotByIndex(int botIndex)
{

}

const BotInfo* const BotManager::getBot(int botID)
{

}

int BotManager::getBotIDByIndex(int index)
{

}

const char* const BotManager::getBotName(int botID)
{

}

bool BotManager::isBotDealer(int botID)
{

}

int BotManager::getBotChips(int botID)
{

}

int BotManager::getBotPot(int botID)
{

}

bool BotManager::isBotHandRevealed(int botID)
{

}

int BotManager::getBotLang(int botID)
{

}

bool BotManager::isBotInGame(int botID)
{

}

bool BotManager::isBotInRound(bool botID)
{

}

const Card* const BotManager::lookAtBotHand(int botID, int cardIndex)
{

}

int BotManager::getReservedCredit()
{

}

const Table* const BotManager::getTable()
{

}

int BotManager::getCallAmount()
{

}

const Card* const BotManager::getTableCard(int cardIndex)
{

}

int BotManager::getMinRaise()
{

}

int BotManager::getPotSum()
{

}

int BotManager::getBigBlind(int blindIndex)
{

}

int BotManager::getBigBlindAtRound(int round)
{

}

int BotManager::getBlindShiftDeadline(int shiftDeadlineIndex)
{

}

int BotManager::getNextBlindShiftDeadline()
{

}

int BotManager::getRebuyDeadline()
{

}

int BotManager::getSmallBlind(int blindIndex)
{

}

int BotManager::getSmallBlindAtRound(int round)
{

}

bool BotManager::raise(int raiseAmount)
{

}

bool BotManager::canRebuy(int rebuyAmount)
{

}

bool BotManager::rebuy(int rebuyAmount)
{

}

void BotManager::talk(Comment comment)
{

}

BotKnowledgeHandler* const BotManager::getBotKnowledge()
{

}

int BotManager::addKnowledgeTableRow(int tableID)
{

}

int BotManager::createKnowledgeTable(int numOfCols, int* colTypes)
{

}

bool BotManager::deleteKnowledgeTableData(int tableID, int row, int col)
{

}

KnowledgeTable* const BotManager::getKnowledgeTable(int tableID)
{

}

KnowledgeDataType BotManager::getKnowledgeTableDataType(int tableID, int col)
{

}

T BotManager::getKnowledgeTableData<T>(int tableID, int row, int col, bool* error = null)
{

}

int BotManager::getKnowledgeTableDataInt(int tableID, int row, int col, bool* error = null)
{

}

bool BotManager::getKnowledgeTableDataBool(int tableID, int row, int col, bool* error = null)
{

}

char BotManager::getKnowledgeTableDataChar(int tableID, int row, int col, bool* error = null)
{

}

char* BotManager::getKnowledgeTableDataString(int tableID, int row, int col, bool* error = null)
{

}

float BotManager::getKnowledgeTableDataFloat(int tableID, int row, int col, bool* error = null)
{

}

int BotManager::getKnowledgeTableNumOfCols(int tableID)
{

}

int BotManager::getKnowledgeTableNumOfRows(int tableID)
{

}

bool BotManager::removeKnowledgeTable(int tableID)
{

}

bool BotManager::removeKnowledgeTableRow(int tableID, int row)
{

}

void BotManager::setKnowledgeTableData<T>(int T, int tableID, int row, int col, bool* error = null)
{

}

void BotManager::setKnowledgeTableDataInt(int int, int tableID, int row, int col, bool* error = null)
{

}

void BotManager::setKnowledgeTableDataBool(int bool, int tableID, int row, int col, bool* error = null)
{

}

void BotManager::setKnowledgeTableDataChar(int char, int tableID, int row, int col, bool* error = null)
{

}

void BotManager::setKnowledgeTableDataString(int char*, int tableID, int row, int col, bool* error = null)
{

}

void BotManager::setKnowledgeTableDataFloat(int float, int tableID, int row, int col, bool* error = null)
{

}

const BotInfo* const BotManager::getBotToTheLeft(int nth)
{

}

const BotInfo* const BotManager::getBotToTheRight(int nth)
{

}

int BotManager::getNumOfBots()
{

}

long BotManager::getAllowedBotCalcTime()
{

}

int BotManager::getStartingChips()
{

}

const char* const BotManager::getName()
{

}

int BotManager::getLang()
{

}

void BotManager::quit()
{

}

bool BotManager::rebuyOrLeave()
{

}

void BotManager::step()
{

}

const char* const BotManager::getName()
{

}

int BotManager::getLang()
{

}

void BotManager::receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* const data)
{

}

void BotManager::receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* const data)
{

}

void BotManager::receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* const data)
{

}

