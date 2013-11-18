#include "TestCommunicator.h"
#include "BotLanguage.h"
#include "Card.h"
#include "Comment.h"
#include "KnowledgeDataType.h"
#include "BettingSystem.h"
#include "HandRank.h"
#include "BroadcastMessage.h"

bool TestCommunicator::allin()
{

}

bool TestCommunicator::call(bool allInIfNecessary = false)
{

}

bool TestCommunicator::canAllin()
{

}

bool TestCommunicator::canCall(bool allInIfNecessary = false)
{

}

bool TestCommunicator::canCheck()
{

}

bool TestCommunicator::canFold()
{

}

bool TestCommunicator::canRaise(int raiseAmount)
{

}

bool TestCommunicator::canRebuy(int rebuyAmount)
{

}

bool TestCommunicator::canTalk()
{

}

bool TestCommunicator::canStep()
{

}

bool TestCommunicator::check()
{

}

bool TestCommunicator::fold()
{

}

int TestCommunicator::getNumOfBots(bool onlyInGame = false, bool onlyInRound = false)
{

}

int TestCommunicator::getBotIDByIndex(int index)
{

}

int TestCommunicator::getBotIDToTheLeft(int nth, bool onlyInGame = false, bool onlyInRound = false)
{

}

int TestCommunicator::getBotIDToTheRight(int nth, bool onlyInGame = false, bool onlyInRound = false)
{

}

string TestCommunicator::getBotName(int botID)
{

}

bool TestCommunicator::isBotDealer(int botID)
{

}

bool TestCommunicator::isEmotionAllowed()
{

}

int TestCommunicator::getBotChips(int botID)
{

}

int TestCommunicator::getBotPot(int botID)
{

}

bool TestCommunicator::isBotHandRevealed(int botID)
{

}

BotLanguage TestCommunicator::getBotLang(int botID)
{

}

int TestCommunicator::getMaxNumOfRaises()
{

}

int TestCommunicator::getBotIndexByID(int botID)
{

}

bool TestCommunicator::isBotInGame(int botID)
{

}

bool TestCommunicator::isBotInRound(bool botID)
{

}

Card TestCommunicator::lookAtBotHand(int botID, int cardIndex)
{

}

int TestCommunicator::getReservedCredit()
{

}

int TestCommunicator::getCallAmount()
{

}

int TestCommunicator::getTableNumOfCards()
{

}

Card TestCommunicator::getTableCard(int cardIndex)
{

}

int TestCommunicator::getMinRaise()
{

}

int TestCommunicator::getPotSum()
{

}

int TestCommunicator::getBigBlind(int blindIndex)
{

}

int TestCommunicator::getBigBlindAtRound(int round = currentRound)
{

}

int TestCommunicator::getBlindShiftDeadline(int shiftDeadlineIndex)
{

}

int TestCommunicator::getNextBlindShiftDeadline()
{

}

int TestCommunicator::getRebuyDeadline()
{

}

int TestCommunicator::getSmallBlind(int blindIndex)
{

}

int TestCommunicator::getSmallBlindAtRound(int round = currentRound)
{

}

bool TestCommunicator::raise(int raiseAmount)
{

}

bool TestCommunicator::rebuy(int rebuyAmount)
{

}

bool TestCommunicator::talk(Comment comment)
{

}

int TestCommunicator::addKnowledgeTableRow(int tableID)
{

}

int TestCommunicator::createKnowledgeTable(int numOfCols, list<KnowledgeDataType> colTypes)
{

}

KnowledgeDataType TestCommunicator::getKnowledgeTableDataType(int tableID, int col)
{

}

bool TestCommunicator::getKnowledgeTableData(int& val, int tableID, int row, int col)
{

}

bool TestCommunicator::getKnowledgeTableData(bool& val, int tableID, int row, int col)
{

}

bool TestCommunicator::getKnowledgeTableData(char& val, int tableID, int row, int col)
{

}

bool TestCommunicator::getKnowledgeTableData(string& val, int tableID, int row, int col)
{

}

bool TestCommunicator::getKnowledgeTableData(float& val, int tableID, int row, int col)
{

}

int TestCommunicator::getKnowledgeTableNumOfCols(int tableID)
{

}

int TestCommunicator::getKnowledgeTableNumOfRows(int tableID)
{

}

bool TestCommunicator::removeKnowledgeTable(int tableID)
{

}

bool TestCommunicator::removeKnowledgeTableRow(int tableID, int row)
{

}

bool TestCommunicator::setKnowledgeTableData(int int, int tableID, int row, int col)
{

}

bool TestCommunicator::setKnowledgeTableData(int bool, int tableID, int row, int col)
{

}

bool TestCommunicator::setKnowledgeTableData(int char, int tableID, int row, int col)
{

}

bool TestCommunicator::setKnowledgeTableData(int string, int tableID, int row, int col)
{

}

bool TestCommunicator::setKnowledgeTableData(int float, int tableID, int row, int col)
{

}

long TestCommunicator::getAllowedBotCalcTime()
{

}

int TestCommunicator::getStartingChips()
{

}

int TestCommunicator::genNumOfBlinds()
{

}

int TestCommunicator::getNumOfRebuysAllowed()
{

}

bool TestCommunicator::isTalkAllowed()
{

}

bool TestCommunicator::isBotKnowledgeUseAllowed()
{

}

int TestCommunicator::getStartingChips()
{

}

int TestCommunicator::getNumOfRebuys()
{

}

int TestCommunicator::getCurrentRound()
{

}

BettingSystem TestCommunicator::getBettingSystem()
{

}

HandRank TestCommunicator::getHandRank()
{

}

int TestCommunicator::getBotID()
{

}

const char* const TestCommunicator::getName()
{

}

int TestCommunicator::getLang()
{

}

void TestCommunicator::receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const shared_ptr<int> data)
{

}

void TestCommunicator::receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const shared_ptr<int> data)
{

}

