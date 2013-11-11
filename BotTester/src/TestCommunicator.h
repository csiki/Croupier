#if !defined(_TESTCOMMUNICATOR_H)
#define _TESTCOMMUNICATOR_H

#include "BotCommunicator.h"
#include "BotLanguage.h"
#include "Card.h"
#include "Comment.h"
#include "KnowledgeDataType.h"
#include "BettingSystem.h"
#include "HandRank.h"
#include "BroadcastMessage.h"

class TestCommunicator : public BotCommunicator
{
public:
    bool allin();
    bool call(bool allInIfNecessary = false);
    bool canAllin();
    bool canCall(bool allInIfNecessary = false);
    bool canCheck();
    bool canFold();
    bool canRaise(int raiseAmount);
    bool canRebuy(int rebuyAmount);
    bool canTalk();
    bool canStep();
    bool check();
    bool fold();
    int getNumOfBots(bool onlyInGame = false, bool onlyInRound = false);
    int getBotIDByIndex(int index);
    int getBotIDToTheLeft(int nth, bool onlyInGame = false, bool onlyInRound = false);
    int getBotIDToTheRight(int nth, bool onlyInGame = false, bool onlyInRound = false);
    string getBotName(int botID);
    bool isBotDealer(int botID);
    bool isEmotionAllowed();
    int getBotChips(int botID);
    int getBotPot(int botID);
    bool isBotHandRevealed(int botID);
    BotLanguage getBotLang(int botID);
    int getMaxNumOfRaises();
    int getBotIndexByID(int botID);
    bool isBotInGame(int botID);
    bool isBotInRound(bool botID);
    Card lookAtBotHand(int botID, int cardIndex);
    int getReservedCredit();
    int getCallAmount();
    int getTableNumOfCards();
    Card getTableCard(int cardIndex);
    int getMinRaise();
    int getPotSum();
    int getBigBlind(int blindIndex);
    int getBigBlindAtRound(int round = currentRound);
    int getBlindShiftDeadline(int shiftDeadlineIndex);
    int getNextBlindShiftDeadline();
    int getRebuyDeadline();
    int getSmallBlind(int blindIndex);
    int getSmallBlindAtRound(int round = currentRound);
    bool raise(int raiseAmount);
    bool rebuy(int rebuyAmount);
    bool talk(Comment comment);
    int addKnowledgeTableRow(int tableID);
    int createKnowledgeTable(int numOfCols, list<KnowledgeDataType> colTypes);
    KnowledgeDataType getKnowledgeTableDataType(int tableID, int col);
    bool getKnowledgeTableData(int& val, int tableID, int row, int col);
    bool getKnowledgeTableData(bool& val, int tableID, int row, int col);
    bool getKnowledgeTableData(char& val, int tableID, int row, int col);
    bool getKnowledgeTableData(string& val, int tableID, int row, int col);
    bool getKnowledgeTableData(float& val, int tableID, int row, int col);
    int getKnowledgeTableNumOfCols(int tableID);
    int getKnowledgeTableNumOfRows(int tableID);
    bool removeKnowledgeTable(int tableID);
    bool removeKnowledgeTableRow(int tableID, int row);
    bool setKnowledgeTableData(int int, int tableID, int row, int col);
    bool setKnowledgeTableData(int bool, int tableID, int row, int col);
    bool setKnowledgeTableData(int char, int tableID, int row, int col);
    bool setKnowledgeTableData(int string, int tableID, int row, int col);
    bool setKnowledgeTableData(int float, int tableID, int row, int col);
    long getAllowedBotCalcTime();
    int getStartingChips();
    int genNumOfBlinds();
    int getNumOfRebuysAllowed();
    bool isTalkAllowed();
    bool isBotKnowledgeUseAllowed();
    int getStartingChips();
    int getNumOfRebuys();
    int getCurrentRound();
    BettingSystem getBettingSystem();
    HandRank getHandRank();
    int getBotID();
    const char* const getName();
    int getLang();
    void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const shared_ptr<int> data);
    void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const shared_ptr<int> data);
};

#endif  //_TESTCOMMUNICATOR_H
