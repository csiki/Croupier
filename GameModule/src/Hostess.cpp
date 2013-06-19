#include "stdafx.h"
#include "Hostess.h"
#include "BroadcastMessage.h"

void Hostess::fillBotsByID()
{
	botsByID.clear();
	int numOfBots = table->getNumOfBots();
	for (int i = 0; i < numOfBots; i++)
	{
		botsByID.insert(std::pair<int, const BotInfo*>(i, table->getBotByIndex(i)));
	}
}

/** Returns the maximum of the pots on table (by AIs).
*/
int Hostess::getCallAmount() const
{
	return callAmount;
}

int Hostess::getMinRaise() const
{
	return 0;
}

int Hostess::getBigBlindAtRound(int round) const
{
	return 0;
}

int Hostess::getNextBlindShiftDeadline() const
{
	return 0;
}

int Hostess::getSmallBlindAtRound(int round) const
{
	return 0;
}

const BotInfo* Hostess::getBotByID(int botID) const
{
	std::pair<const int, const BotInfo*> p = *botsByID.find(botID);
	return p.second;
}

int Hostess::getBotIDByIndex(int index) const
{
	for (int i = 0; i < botsByID; i++)
	{

	}
}

void Hostess::receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data)
{

}

int Hostess::getCurrentRound() const
{
	return 0;
}

int Hostess::getBotIDToTheRight(int from, int nth, bool onlyInRound) const
{
	return 0;
}

int Hostess::getBotIDToTheLeft(int from, int nth, bool onlyInRound) const
{
	return 0;
}

int Hostess::getNumOfBots(bool onlyInGame, bool onlyInRound) const
{
	return 0;
}

bool Hostess::canRaise() const
{
	return 0;
}