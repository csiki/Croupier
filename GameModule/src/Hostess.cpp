#include "stdafx.h"
#include "Hostess.h"
#include "BroadcastMessage.h"

void Hostess::fillBotsByID()
{
}

/** Returns the maximum of the pots on table (by AIs).
*/
int Hostess::getCallAmount() const
{
	return 0;
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
	return 0;
}

int Hostess::getBotIDByIndex(int index) const
{
	return 0;
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