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
	Table* const table;
	int callAmount;
	map<int,BotInfo*> botsByID;
	Rulz *rules;
public:
	int getCallAmount();
	int getMinRaise();
	int getBigBlindAtRound(int round);
	int getNextBlindShiftDeadline();
	int getSmallBlindAtRound(int round);
	BotInfo* getBotToTheLeft(int nth);
	BotInfo* getBotToTheRight(int nth);
	BotInfo* getBotByID(int botID);
	int getBotIDByIndex(int index);
	int getNumOfBots(bool onlyInRound = false);
	void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data);
};

#endif  //_HOSTESS_H
