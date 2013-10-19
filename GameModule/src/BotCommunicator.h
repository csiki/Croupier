#if !defined(_BOTCOMMUNICATOR_H)
#define _BOTCOMMUNICATOR_H

#include "BotInfo.h"
#include "Comment.h"
#include "KnowledgeDataType.h"
#include "BotKnowledgeHandler.h"
#include "KnowledgeTable.h"
#include "Table.h"
#include "Emotion.h"
#include "HandRank.h"

/**	Communication interface for AIs.
*/
class BotCommunicator : virtual public BotInfo
{
public:

	BotCommunicator(int playerID, BroadcastStation* broadcastStation, Loggable* loggable, int chips) :
		BotInfo(playerID, broadcastStation, loggable, chips), Entity(playerID)
	{}

	// botmanager
	Card getHand(int cardIndex) const;
	void setEmotion(Emotion emotion);
	virtual int getReservedCredit() const = 0;
	virtual int getNumOfRebuys() const = 0;
	// bot
	virtual int getBotID() const = 0;
	virtual std::string getName() const = 0;
	virtual BotLanguage getLang() const = 0;
	// other bot
	virtual Emotion getBotEmotion(int botID) const = 0;
	virtual std::string getBotName(int botID) const = 0;
	virtual bool isBotDealer(int botID) const = 0;
	virtual int getBotChips(int botID) const = 0;
	virtual int getBotPot(int botID) const = 0;
	virtual bool isBotHandRevealed(int botID) const = 0;
	virtual BotLanguage getBotLang(int botID) const = 0;
	virtual bool isBotInGame(int botID) const = 0;
	virtual bool isBotInRound(bool botID) const = 0;
	virtual Card lookAtBotHand(int botID, int cardIndex) const = 0;
	// move
	virtual bool canTalk() const = 0;
	virtual bool canStep() const = 0;
	virtual bool allin() = 0;
	virtual bool call() = 0;
	virtual bool canAllin() const = 0;
	virtual bool canCall() const = 0;
	virtual bool canCheck() const = 0;
	virtual bool canFold() const = 0;
	virtual bool canRaise(int raiseAmount) const = 0;
	virtual bool check() = 0;
	virtual bool fold() = 0;
	virtual bool raise(int raiseAmount) = 0;
	virtual bool canRebuy(int rebuyAmount) const = 0;
	virtual bool rebuy(int rebuyAmount) = 0;
	virtual bool talk(Comment comment) = 0;
	virtual void quit() = 0;
	// hostess
	virtual int getNumOfBots(bool onlyInGame = false, bool onlyInRound = false) const = 0;
	virtual int getBotIDByIndex(int index) const = 0;
	virtual int getBotIndexByID(int botID) const = 0;
	virtual int getBotIDToTheRight(int nth = 1, bool onlyInGame = false, bool onlyInRound = false) const = 0;
	virtual int getBotIDToTheLeft(int nth = 1, bool onlyInGame = false, bool onlyInRound = false) const = 0;
	virtual int getCallAmount() const = 0;
	virtual int getMinRaise() const = 0;
	virtual int getBigBlindAtRound(int round = -1) const = 0; // -1 means current round
	virtual int getBlindShiftDeadline(int shiftDeadlineIndex) const = 0;
	virtual int getNextBlindShiftDeadline() const = 0;
	virtual int getSmallBlindAtRound(int round = -1) const = 0; // -1 means current round
	virtual int getCurrentRound() const = 0;
	virtual HandRank getHandRank() const = 0;
	// table
	virtual int getTableNumOfCards() const = 0;
	virtual Card getTableCard(int cardIndex) const = 0;
	virtual int getPotSum() const = 0;
	// rulz
	virtual int getBigBlind(int blindIndex) const = 0;
	virtual int getRebuyDeadline() const = 0;
	virtual int getSmallBlind(int blindIndex) const = 0;
	virtual int getAllowedBotCalcTime() const = 0;
	virtual int getStartingChips() const = 0;
	virtual int getNumOfBlinds() const = 0;
	virtual int getNumOfRebuysAllowed() const = 0;
	virtual bool isTalkAllowed() const = 0;
	virtual bool isEmotionAllowed() const = 0;
	virtual bool isBotKnowledgeUseAllowed() const = 0;
	// knowledge handler
	virtual bool getKnowledgeTableData(int& val, int tableID, int row, int col) const = 0;
	virtual bool getKnowledgeTableData(bool& val, int tableID, int row, int col) const = 0;
	virtual bool getKnowledgeTableData(char& val, int tableID, int row, int col) const = 0;
	virtual bool getKnowledgeTableData(std::string& val, int tableID, int row, int col) const = 0;
	virtual bool getKnowledgeTableData(float& val, int tableID, int row, int col) const = 0;

	virtual bool setKnowledgeTableData(int val, int tableID, int row, int col) = 0;
	virtual bool setKnowledgeTableData(bool val, int tableID, int row, int col) = 0;
	virtual bool setKnowledgeTableData(char val, int tableID, int row, int col) = 0;
	virtual bool setKnowledgeTableData(const char* val, int tableID, int row, int col) = 0;
	virtual bool setKnowledgeTableData(std::string val, int tableID, int row, int col) = 0;
	virtual bool setKnowledgeTableData(float val, int tableID, int row, int col) = 0;

	virtual KnowledgeDataType getKnowledgeTableDataType(int tableID, int col) const = 0;
	virtual int addKnowledgeTableRow(int tableID) = 0;
	virtual int createKnowledgeTable(int numOfCols, std::list<KnowledgeDataType> colTypes) = 0;
	virtual int getKnowledgeTableNumOfCols(int tableID) const = 0;
	virtual int getKnowledgeTableNumOfRows(int tableID) const = 0;
	virtual bool removeKnowledgeTable(int tableID) = 0;
	virtual bool removeKnowledgeTableRow(int tableID, int row) = 0;
	virtual bool isTableLoaded(int tableID) const = 0;
};

#endif  //_BOTCOMMUNICATOR_H
