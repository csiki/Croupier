#if !defined(_BOTMANAGER_H)
#define _BOTMANAGER_H

#include "BotCommunicator.h"
#include "BotHandler.h"
#include "Logger.h"
#include "BroadcastMember.h"
#include "BotKnowledgeHandler.h"
#include "Bot.h"
#include "Table.h"
#include "BroadcastStation.h"
#include "Hostess.h"
#include "Rulz.h"
#include "Comment.h"
#include "KnowledgeDataType.h"
#include "BroadcastMessage.h"

class BotManager : public BotCommunicator, public BotHandler, public Logger, public BroadcastMember, public Logger
{
private:
	int numOfRebuys;
	int reservedCredit;
	BotKnowledgeHandler *bkHandler;
	Bot *bot;
	Table *table;
	BroadcastStation *broadcast;
	Hostess *hostess;
	Rulz *rules;
public:
	bool allin();
	bool call();
	bool canAllin();
	bool canCall();
	bool canCheck();
	bool canFold();
	bool canRaise(int raiseAmount);
	bool canRebuy(int rebuyAmount);
	bool check();
	bool fold();
	int getNumOfBots(bool onlyInRound = false);
	const BotInfo* const getBotByIndex(int botIndex);
	const BotInfo* const getBot(int botID);
	int getBotIDByIndex(int index);
	const char* const getBotName(int botID);
	bool isBotDealer(int botID);
	int getBotChips(int botID);
	int getBotPot(int botID);
	bool isBotHandRevealed(int botID);
	int getBotLang(int botID);
	bool isBotInGame(int botID);
	bool isBotInRound(bool botID);
	const Card* const lookAtBotHand(int botID, int cardIndex);
	int getReservedCredit();
	const Table* const getTable();
	int getCallAmount();
	const Card* const getTableCard(int cardIndex);
	int getMinRaise();
	int getPotSum();
	int getBigBlind(int blindIndex);
	int getBigBlindAtRound(int round);
	int getBlindShiftDeadline(int shiftDeadlineIndex);
	int getNextBlindShiftDeadline();
	int getRebuyDeadline();
	int getSmallBlind(int blindIndex);
	int getSmallBlindAtRound(int round);
	bool raise(int raiseAmount);
	bool canRebuy(int rebuyAmount);
	bool rebuy(int rebuyAmount);
	void talk(Comment comment);
	BotKnowledgeHandler* const getBotKnowledge();
	int addKnowledgeTableRow(int tableID);
	int createKnowledgeTable(int numOfCols, int* colTypes);
	bool deleteKnowledgeTableData(int tableID, int row, int col);
	KnowledgeTable* const getKnowledgeTable(int tableID);
	KnowledgeDataType getKnowledgeTableDataType(int tableID, int col);
	T getKnowledgeTableData<T>(int tableID, int row, int col, bool* error = null);
	int getKnowledgeTableDataInt(int tableID, int row, int col, bool* error = null);
	bool getKnowledgeTableDataBool(int tableID, int row, int col, bool* error = null);
	char getKnowledgeTableDataChar(int tableID, int row, int col, bool* error = null);
	char* getKnowledgeTableDataString(int tableID, int row, int col, bool* error = null);
	float getKnowledgeTableDataFloat(int tableID, int row, int col, bool* error = null);
	int getKnowledgeTableNumOfCols(int tableID);
	int getKnowledgeTableNumOfRows(int tableID);
	bool removeKnowledgeTable(int tableID);
	bool removeKnowledgeTableRow(int tableID, int row);
	void setKnowledgeTableData<T>(int T, int tableID, int row, int col, bool* error = null);
	void setKnowledgeTableDataInt(int int, int tableID, int row, int col, bool* error = null);
	void setKnowledgeTableDataBool(int bool, int tableID, int row, int col, bool* error = null);
	void setKnowledgeTableDataChar(int char, int tableID, int row, int col, bool* error = null);
	void setKnowledgeTableDataString(int char*, int tableID, int row, int col, bool* error = null);
	void setKnowledgeTableDataFloat(int float, int tableID, int row, int col, bool* error = null);
	const BotInfo* const getBotToTheLeft(int nth);
	const BotInfo* const getBotToTheRight(int nth);
	int getNumOfBots();
	long getAllowedBotCalcTime();
	int getStartingChips();
	const char* const getName();
	int getLang();
	void quit();
	bool rebuyOrLeave();
	void step();
	const char* const getName();
	int getLang();
	void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* const data);
	void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* const data);
	void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* const data);
};

#endif  //_BOTMANAGER_H
