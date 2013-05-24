#if !defined(_HOSTESS_H)
#define _HOSTESS_H

#include "BroadcastMember.h"
#include "Table.h"
#include "Rulz.h"
#include "BroadcastMessage.h"

class Hostess : public BroadcastMember
{
private:
	Table* const table;
	int callAmount;
	map<int,BotInfo*> botsByID;
	Table *table;
	Rulz *rules;
	Rulz *rules;
public:
	int getCallAmount();
	int getMinRaise();
	int getBigBlindAtRound(int round);
	int getNextBlindShiftDeadline();
	int getSmallBlindAtRound(int round);
	const BotInfo* const getBotToTheLeft(int nth);
	const BotInfo* const getBotToTheRight(int nth);
	const BotInfo* const getBotByID(int botID);
	int getBotIDByIndex(int index);
	int getNumOfBots(bool onlyInRound = false);
	void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* const data);
};

#endif  //_HOSTESS_H
