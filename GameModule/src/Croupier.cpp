#include "stdafx.h"
#include "Croupier.h"
#include "BroadcastMessage.h"

/** Burn a card.
*/
void Croupier::burn(Card* c)
{
	if (numberOfBurntCards >= 3)
	{
		throw "Too much card to burn!"; // TEST nem jöhet létre, csak tesztelésnek van itt
	}

	this->burnt[this->numberOfBurntCards++] = c;
}

/** Puts burnt cards, cards from the table, and hands back to deck.
*/
void Croupier::collectCards()
{
	// burnt cards
	for (int i = 0; i < this->numberOfBurntCards; ++i)
	{
		this->deck->push(this->burnt[i]);
		this->burnt[i] = 0;
	}
	this->numberOfBurntCards = 0;

	// cards from table
	for (int i = 0; i < this->table->getNumOfCards(); ++i)
	{
		this->deck->push(this->table->rmCard(i));
	}

	// hands
	for (int i = 0; i < this->numOfBots; ++i)
	{
		if (this->bots[i]->isInGame())
		{
			// if a bot is in game, then it must have cards
			this->deck->push(this->bots[i]->takeHand(0));
			this->deck->push(this->bots[i]->takeHand(1));
		}
	}
}

void Croupier::betRound()
{
	// TODO
}

void Croupier::dealing()
{
	// TODO
}

int Croupier::nextActiveBot(int from) const
{
	if (from == -1) // default value
	{
		from = this->currentBotIndex;
	}

	int offset = 1;
	while (!this->bots[(from + offset) % this->numOfBots]->isInRound)
	{
		++offset; // iterate through bots till we found an active

		if (offset > numOfBots)
		{
			throw "No active = in round bots"; // TEST törölni késõbb
		}
	}

	return (from + offset) % this->numOfBots;
}

void Croupier::preflop()
{
	// TODO
}

void Croupier::flop()
{
	// TODO cards to table, broadcast, betRound()
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


