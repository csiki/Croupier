#if !defined(_HOSTESS_H)
#define _HOSTESS_H

#include "BroadcastMember.h"
#include "Table.h"
#include "Rulz.h"
#include "BroadcastMessage.h"
#include <map>
#include "BotInfo.h"

class Hostess : public BroadcastMember
{
private:
	const Table* table;
	const Rulz* rules;
	map<int, const BotInfo*> botsByID;
	int callAmount;
public:
	int getCallAmount();
	int getMinRaise();
	int getBigBlindAtRound(int round);
	int getNextBlindShiftDeadline();
	int getSmallBlindAtRound(int round);
	const BotInfo* getBotToTheLeft(int nth);
	const BotInfo* getBotToTheRight(int nth);
	const BotInfo* getBotByID(int botID);
	int getBotIDByIndex(int index);
	int getNumOfBots(bool onlyInRound = false);
	void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data);
};

#endif  //_HOSTESS_H
