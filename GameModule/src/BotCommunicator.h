#if !defined(_BOTCOMMUNICATOR_H)
#define _BOTCOMMUNICATOR_H

#include "BotInfo.h"
#include "Comment.h"
#include "KnowledgeDataType.h"
#include "BotKnowledgeHandler.h"
#include "KnowledgeTable.h"
#include "Table.h"
#include "Emotion.h"

/**	Communication interface for AIs.
*/
class BotCommunicator : public BotInfo
{
public:
	// botmanager
	Card getHand(int cardIndex);
	void setEmotion(Emotion emotion);
	virtual int getReservedCredit() = 0;
	virtual int getNumOfRebuys() = 0;
	// bot
	virtual int getID() = 0;
	virtual std::string getName() = 0;
	virtual int getLang() = 0;
	// other bot
	virtual Emotion getBotEmotion(int botID) const;
	virtual std::string getBotName(int botID) = 0;
	virtual bool isBotDealer(int botID) = 0;
	virtual int getBotChips(int botID) = 0;
	virtual int getBotPot(int botID) = 0;
	virtual bool isBotHandRevealed(int botID) = 0;
	virtual int getBotLang(int botID) = 0;
	virtual bool isBotInGame(int botID) = 0;
	virtual bool isBotInRound(bool botID) = 0;
	virtual Card lookAtBotHand(int botID, int cardIndex) = 0;
	// move
	virtual bool allin() = 0;
	virtual bool call() = 0;
	virtual bool canAllin() = 0;
	virtual bool canCall() = 0;
	virtual bool canCheck() = 0;
	virtual bool canFold() = 0;
	virtual bool canRaise(int raiseAmount) = 0;
	virtual bool check() = 0;
	virtual bool fold() = 0;
	virtual bool raise(int raiseAmount) = 0;
	virtual bool canRebuy(int rebuyAmount) = 0;
	virtual bool rebuy(int rebuyAmount) = 0;
	virtual void talk(Comment comment) = 0;
	// hostess
	virtual int getNumOfBots(bool onlyInGame = false, bool onlyInRound = false) = 0;
	virtual int getBotIDByIndex(int index) = 0;
	virtual int getBotIDToTheRight(int nth = 1, bool onlyInRound = false) = 0;
	virtual int getBotIDToTheLeft(int nth = 1, bool onlyInRound = false) = 0;
	virtual int getCallAmount() = 0;
	virtual int getMinRaise() = 0;
	virtual int getBigBlindAtRound(int round) = 0;
	virtual int getBlindShiftDeadline(int shiftDeadlineIndex) = 0;
	virtual int getNextBlindShiftDeadline() = 0;
	virtual int getSmallBlindAtRound(int round) = 0;
	// table
	virtual int getTableNumOfCards();
	virtual Card getTableCard(int cardIndex) = 0;
	virtual int getPotSum() = 0;
	// rulz
	virtual int getBigBlind(int blindIndex) = 0;
	virtual int getRebuyDeadline() = 0;
	virtual int getSmallBlind(int blindIndex) = 0;
	virtual long getAllowedBotCalcTime() = 0;
	virtual int getStartingChips() = 0;
	virtual int genNumOfBlinds() = 0;
	virtual int getNumOfRebuysAllowed() = 0;
	virtual bool isTalkAllowed() = 0;
	virtual bool isBotKnowledgeUseAllowed() = 0;
	// knowledge handler
	virtual int addKnowledgeTableRow(int tableID) = 0;
	virtual int createKnowledgeTable(int numOfCols, KnowledgeDataType* colTypes) = 0;
	virtual KnowledgeDataType getKnowledgeTableDataType(int tableID, int col) = 0;
	virtual int getKnowledgeTableDataInt(int tableID, int row, int col, bool* error = 0) = 0;
	virtual bool getKnowledgeTableDataBool(int tableID, int row, int col, bool* error = 0) = 0;
	virtual char getKnowledgeTableDataChar(int tableID, int row, int col, bool* error = 0) = 0;
	virtual std::string getKnowledgeTableDataString(int tableID, int row, int col, bool* error = 0) = 0;
	virtual float getKnowledgeTableDataFloat(int tableID, int row, int col, bool* error = 0) = 0;
	virtual int getKnowledgeTableNumOfCols(int tableID) = 0;
	virtual int getKnowledgeTableNumOfRows(int tableID) = 0;
	virtual bool removeKnowledgeTable(int tableID) = 0;
	virtual bool removeKnowledgeTableRow(int tableID, int row) = 0;
	virtual void setKnowledgeTableData(int val, int tableID, int row, int col, bool* error = 0) = 0;
	virtual void setKnowledgeTableData(bool val, int tableID, int row, int col, bool* error = 0) = 0;
	virtual void setKnowledgeTableData(char val, int tableID, int row, int col, bool* error = 0) = 0;
	virtual void setKnowledgeTableData(char* val, int tableID, int row, int col, bool* error = 0) = 0;
	virtual void setKnowledgeTableData(std::string val, int tableID, int row, int col, bool* error = 0) = 0; // same as char*
	virtual void setKnowledgeTableData(float val, int tableID, int row, int col, bool* error = 0) = 0;
};

#endif  //_BOTCOMMUNICATOR_H
