#include "stdafx.h"
#include "Croupier.h"
#include "BroadcastMessage.h"

void Croupier::burn(Card* c)
{
	
}

void Croupier::collectCards()
{

}

void Croupier::betRound()
{

}

void Croupier::dealing()
{

}

int Croupier::nextActiveBot(int from) const
{
	return 0;
}

void Croupier::preflop()
{

}

void Croupier::flop()
{

}

void Croupier::turn()
{

}

void Croupier::river()
{

}

void Croupier::showdown()
{

}

void Croupier::handOutPot(int winnerIndex)
{

}

void Croupier::handOutPot(int numOfWinners, const int* winnersIndex)
{

}

void Croupier::refreshBlinds()
{

}

void Croupier::determineWinners(int numOfWinners, int* winnersIndex)
{

}

void Croupier::letsPoker()
{

}

int Croupier::getKickAtRound(int botIndex) const
{
	return 0;
}

int Croupier::getBotIDByIndex(int botIndex) const
{
	return 0;
}

void Croupier::provideBotHandlers(int numOfBots, BotHandler** bhs)
{

}

void Croupier::provideTable(const Table* table)
{

}

void Croupier::receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data)
{

}


