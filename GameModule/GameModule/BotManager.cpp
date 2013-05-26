#include "stdafx.h"
#include "BotManager.h"
#include "Comment.h"
#include "KnowledgeDataType.h"
#include "BroadcastMessage.h"

bool BotManager::allin()
{
	return 0;
}

bool BotManager::call()
{
	return 0;
}

bool BotManager::canAllin()
{
	return 0;
}

bool BotManager::canCall()
{
	return 0;
}

bool BotManager::canCheck()
{
	return 0;
}

bool BotManager::canFold()
{
	return 0;
}

bool BotManager::canRaise(int raiseAmount)
{
	return 0;
}

bool BotManager::canRebuy(int rebuyAmount)
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

int BotManager::getNumOfBots(bool onlyInRound)
{
	return 0;
}

BotInfo* BotManager::getBotByIndex(int botIndex)
{
	return 0;
}

BotInfo* BotManager::getBot(int botID)
{
	return 0;
}

int BotManager::getBotIDByIndex(int index)
{
	return 0;
}

const char* BotManager::getBotName(int botID)
{
	return 0;
}

bool BotManager::isBotDealer(int botID)
{
	return 0;
}

int BotManager::getBotChips(int botID)
{
	return 0;
}

int BotManager::getBotPot(int botID)
{
	return 0;
}

bool BotManager::isBotHandRevealed(int botID)
{
	return 0;
}

int BotManager::getBotLang(int botID)
{
	return 0;
}

bool BotManager::isBotInGame(int botID)
{
	return 0;
}

bool BotManager::isBotInRound(bool botID)
{
	return 0;
}

Card* BotManager::lookAtBotHand(int botID, int cardIndex)
{
	return 0;
}

int BotManager::getReservedCredit()
{
	return 0;
}

const Table* BotManager::getTable()
{
	return 0;
}

int BotManager::getCallAmount()
{
	return 0;
}

Card* BotManager::getTableCard(int cardIndex)
{
	return 0;
}

int BotManager::getMinRaise()
{
	return 0;
}

int BotManager::getPotSum()
{
	return 0;
}

int BotManager::getBigBlind(int blindIndex)
{
	return 0;
}

int BotManager::getBigBlindAtRound(int round)
{
	return 0;
}

int BotManager::getBlindShiftDeadline(int shiftDeadlineIndex)
{
	return 0;
}

int BotManager::getNextBlindShiftDeadline()
{
	return 0;
}

int BotManager::getRebuyDeadline()
{
	return 0;
}

int BotManager::getSmallBlind(int blindIndex)
{
	return 0;
}

int BotManager::getSmallBlindAtRound(int round)
{
	return 0;
}

bool BotManager::raise(int raiseAmount)
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

BotKnowledgeHandler* BotManager::getBotKnowledge()
{
	return 0;
}

int BotManager::addKnowledgeTableRow(int tableID)
{
	return 0;
}

int BotManager::createKnowledgeTable(int numOfCols, int* colTypes)
{
	return 0;
}

bool BotManager::deleteKnowledgeTableData(int tableID, int row, int col)
{
	return 0;
}

KnowledgeTable* BotManager::getKnowledgeTable(int tableID)
{
	return 0;
}

KnowledgeDataType BotManager::getKnowledgeTableDataType(int tableID, int col)
{
	return KnowledgeDataType::BOOL;
}

int BotManager::getKnowledgeTableDataInt(int tableID, int row, int col, bool* error)
{
	return 0;
}

bool BotManager::getKnowledgeTableDataBool(int tableID, int row, int col, bool* error)
{
	return 0;
}

char BotManager::getKnowledgeTableDataChar(int tableID, int row, int col, bool* error)
{
	return 0;
}

char* BotManager::getKnowledgeTableDataString(int tableID, int row, int col, bool* error)
{
	return 0;
}

float BotManager::getKnowledgeTableDataFloat(int tableID, int row, int col, bool* error)
{
	return 0;
}

int BotManager::getKnowledgeTableNumOfCols(int tableID)
{
	return 0;
}

int BotManager::getKnowledgeTableNumOfRows(int tableID)
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

void BotManager::setKnowledgeTableDataInt(int val, int tableID, int row, int col, bool* error)
{

}

void BotManager::setKnowledgeTableDataBool(bool val, int tableID, int row, int col, bool* error)
{

}

void BotManager::setKnowledgeTableDataChar(char val, int tableID, int row, int col, bool* error)
{

}

void BotManager::setKnowledgeTableDataString(char* val, int tableID, int row, int col, bool* error)
{

}

void BotManager::setKnowledgeTableDataFloat(float val, int tableID, int row, int col, bool* error)
{

}

BotInfo* BotManager::getBotToTheLeft(int nth)
{
	return 0;
}

BotInfo* BotManager::getBotToTheRight(int nth)
{
	return 0;
}

int BotManager::getNumOfBots()
{
	return 0;
}

long BotManager::getAllowedBotCalcTime()
{
	return 0;
}

int BotManager::getStartingChips()
{
	return 0;
}

const char* BotManager::getName()
{
	return 0;
}

int BotManager::getLang()
{
	return 0;
}

void BotManager::quit()
{

}

bool BotManager::rebuyOrLeave()
{
	return 0;
}

void BotManager::step()
{

}

void BotManager::receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data)
{

}

