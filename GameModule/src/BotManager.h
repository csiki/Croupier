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
#include "Comment.h"

// Warning: botID == entityID only when the bot is created and used by the same user.

// TODO idõlimit programozási nyelv alapján minden egyes hívásra
// TODO memórialimit, foglalás módjának meghatározása

/**	Class for managing AIs' communication and validating moves.
*/
class BotManager : public BotCommunicator, public BotHandler, public Logger, public BroadcastMember
{
private:
	int numOfRebuys;
	int reservedCredit;
	BotKnowledgeHandler *bkHandler;
	Bot *bot;
	Hostess *hostess;
	const Table *table;
	const Rulz *rules;
	
	/// Bot's index at table
	int nthAtTable;
public:
	// broadcast
	void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data);
	// botinfo
	int getID() const;
	std::string getName() const;
	int getLang() const;
	// botcommunicator
	int getReservedCredit() const;
	int getNumOfRebuys() const;
	std::string getBotName(int botID) const;
	bool isBotDealer(int botID) const;
	int getBotChips(int botID) const;
	int getBotPot(int botID) const;
	Emotion getBotEmotion(int botID) const;
	bool isBotHandRevealed(int botID) const;
	int getBotLang(int botID) const;
	bool isBotInGame(int botID) const;
	bool isBotInRound(bool botID) const;
	Card lookAtBotHand(int botID, int cardIndex) const;
	bool allin();
	bool call();
	bool canTalk() const;
	bool canStep() const;
	bool canAllin() const;
	bool canCall() const;
	bool canCheck() const;
	bool canFold() const;
	bool canRaise(int raiseAmount) const;
	bool check();
	bool fold();
	bool raise(int raiseAmount);
	bool canRebuy(int rebuyAmount) const;
	bool rebuy(int rebuyAmount);
	void talk(Comment comment);
	int getNumOfBots(bool onlyInGame = false, bool onlyInRound = false) const;
	int getBotIDByIndex(int index) const;
	int getBotIndexByID(int botID) const;
	int getBotIDToTheRight(int nth = 1, bool onlyInGame = false, bool onlyInRound = false) const;
	int getBotIDToTheLeft(int nth = 1, bool onlyInGame = false, bool onlyInRound = false) const;
	int getCallAmount() const;
	int getMinRaise() const;
	int getBigBlindAtRound(int round = -1) const; // -1 means current round
	int getBlindShiftDeadline(int shiftDeadlineIndex) const;
	int getNextBlindShiftDeadline() const;
	int getSmallBlindAtRound(int round = -1) const; // -1 means current round
	int getCurrentRound() const;
	int getTableNumOfCards() const;
	Card getTableCard(int cardIndex) const;
	int getPotSum() const;
	int getBigBlind(int blindIndex) const;
	int getRebuyDeadline() const;
	int getSmallBlind(int blindIndex) const;
	long getAllowedBotCalcTime(int langID) const;
	int getStartingChips() const;
	int genNumOfBlinds() const;
	int getNumOfRebuysAllowed() const;
	bool isTalkAllowed() const;
	bool isEmotionAllowed() const;
	bool isBotKnowledgeUseAllowed() const;
	int addKnowledgeTableRow(int tableID);
	int createKnowledgeTable(int numOfCols, KnowledgeDataType* colTypes);
	KnowledgeDataType getKnowledgeTableDataType(int tableID, int col) const;
	int getKnowledgeTableNumOfCols(int tableID) const;
	int getKnowledgeTableNumOfRows(int tableID) const;
	bool removeKnowledgeTable(int tableID);
	bool removeKnowledgeTableRow(int tableID, int row);
	int getMaxNumOfRaises() const;

	bool getKnowledgeTableData(int& val, int tableID, int row, int col) const;
	bool getKnowledgeTableData(bool& val, int tableID, int row, int col) const;
	bool getKnowledgeTableData(char& val, int tableID, int row, int col) const;
	bool getKnowledgeTableData(std::string& val, int tableID, int row, int col) const;
	bool getKnowledgeTableData(float& val, int tableID, int row, int col) const;

	bool setKnowledgeTableData(int val, int tableID, int row, int col);
	bool setKnowledgeTableData(bool val, int tableID, int row, int col);
	bool setKnowledgeTableData(char val, int tableID, int row, int col);
	bool setKnowledgeTableData(std::string val, int tableID, int row, int col);
	bool setKnowledgeTableData(float val, int tableID, int row, int col);
	// bothandler
	void step();
	void leave();
	bool rebuyOrLeave();
};

#endif  //_BOTMANAGER_H
