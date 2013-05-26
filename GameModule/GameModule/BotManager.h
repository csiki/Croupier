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

class BotManager : public BotCommunicator, public BotHandler, public Logger, public BroadcastMember
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
	BotInfo* getBotByIndex(int botIndex);
	BotInfo* getBot(int botID);
	int getBotIDByIndex(int index);
	const char* getBotName(int botID);
	bool isBotDealer(int botID);
	int getBotChips(int botID);
	int getBotPot(int botID);
	bool isBotHandRevealed(int botID);
	int getBotLang(int botID);
	bool isBotInGame(int botID);
	bool isBotInRound(bool botID);
	Card* lookAtBotHand(int botID, int cardIndex);
	int getReservedCredit();
	const Table* getTable();
	int getCallAmount();
	Card* getTableCard(int cardIndex);
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
	bool rebuy(int rebuyAmount);
	void talk(Comment comment);
	BotKnowledgeHandler* getBotKnowledge();
	int addKnowledgeTableRow(int tableID);
	int createKnowledgeTable(int numOfCols, int* colTypes);
	bool deleteKnowledgeTableData(int tableID, int row, int col);
	KnowledgeTable* getKnowledgeTable(int tableID);
	KnowledgeDataType getKnowledgeTableDataType(int tableID, int col);
	int getKnowledgeTableDataInt(int tableID, int row, int col, bool* error = 0);
	bool getKnowledgeTableDataBool(int tableID, int row, int col, bool* error = 0);
	char getKnowledgeTableDataChar(int tableID, int row, int col, bool* error = 0);
	char* getKnowledgeTableDataString(int tableID, int row, int col, bool* error = 0);
	float getKnowledgeTableDataFloat(int tableID, int row, int col, bool* error = 0);
	int getKnowledgeTableNumOfCols(int tableID);
	int getKnowledgeTableNumOfRows(int tableID);
	bool removeKnowledgeTable(int tableID);
	bool removeKnowledgeTableRow(int tableID, int row);
	void setKnowledgeTableDataInt(int val, int tableID, int row, int col, bool* error = 0);
	void setKnowledgeTableDataBool(bool val, int tableID, int row, int col, bool* error = 0);
	void setKnowledgeTableDataChar(char val, int tableID, int row, int col, bool* error = 0);
	void setKnowledgeTableDataString(char* val, int tableID, int row, int col, bool* error = 0);
	void setKnowledgeTableDataFloat(float val, int tableID, int row, int col, bool* error = 0);
	BotInfo* getBotToTheLeft(int nth);
	BotInfo* getBotToTheRight(int nth);
	int getNumOfBots();
	long getAllowedBotCalcTime();
	int getStartingChips();
	const char* getName();
	int getLang();
	void quit();
	bool rebuyOrLeave();
	void step();
	void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data);
};

#endif  //_BOTMANAGER_H
