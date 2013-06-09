#if !defined(_HOSTESS_H)
#define _HOSTESS_H

#include "BroadcastMember.h"
#include "Table.h"
#include "Rulz.h"
#include "BroadcastMessage.h"
#include "BotInfo.h"

/**	Helper for AIs.
*/
class Hostess : public BroadcastMember
{
private:
	const Table* table;
	const Rulz* rules;
	map<int, const BotInfo*> botsByID;
	int callAmount;
	bool* botsInRound;
	bool* botsInGame;

	void fillBotsByID();
public:
	int getCallAmount() const;
	int getMinRaise() const;
	int getBigBlindAtRound(int round) const;
	int getNextBlindShiftDeadline() const;
	int getSmallBlindAtRound(int round) const;
	int getBotIDToTheRight(int nth, bool onlyInRound) const;
	int getBotIDToTheLeft(int nth, bool onlyInRound) const;
	const BotInfo* getBotByID(int botID) const;
	int getBotIDByIndex(int index) const;
	int getNumOfBots(bool onlyInGame, bool onlyInRound) const;
	void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data);
};

#endif  //_HOSTESS_H
