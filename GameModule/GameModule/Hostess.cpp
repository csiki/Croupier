#include "stdafx.h"
#include "Hostess.h"
#include "BroadcastMessage.h"

int Hostess::getCallAmount()
{
	return 0;
}

int Hostess::getMinRaise()
{
	return 0;
}

int Hostess::getBigBlindAtRound(int round)
{
	return 0;
}

int Hostess::getNextBlindShiftDeadline()
{
	return 0;
}

int Hostess::getSmallBlindAtRound(int round)
{
	return 0;
}

const BotInfo* Hostess::getBotToTheLeft(int nth)
{
	return 0;
}

const BotInfo* Hostess::getBotToTheRight(int nth)
{
	return 0;
}

const BotInfo* Hostess::getBotByID(int botID)
{
	return 0;
}

int Hostess::getBotIDByIndex(int index)
{
	return 0;
}

int Hostess::getNumOfBots(bool onlyInRound)
{
	return 0;
}

void Hostess::receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data)
{

}

