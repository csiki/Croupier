#include "stdafx.h"
#include "Table.h"

/** Adds a card to the ones on table (flop, turn, river).
*/
void Table::addCard(Card* c)
{
	this->cards[++this->numOfCards] = c;
}

/** Returns the number of cards on table (flop, turn, river).
*/
int Table::getNumOfCards() const
{
	return this->numOfCards;
}

/** Removes from table (if there's any flop, turn, river).
*/
Card* Table::rmCard()
{
	if (numOfCards > 0)
	{
		--numOfCards;
		Card* card = this->cards[numOfCards];
		this->cards[numOfCards] = nullptr;
	}

	return nullptr;
}

/** Returns a card from table (flop, turn, river).
*/
const Card* Table::getCard(int index) const
{
	if (index >= 0 && index < this->numOfCards)
	{
		return this->cards[index];
	}

	return nullptr;
}

/** Returns a reference to a bot by its index (at table).
*/
const BotInfo* Table::getBotByIndex(int index) const
{
	if (index >= 0 && index < this->numOfBots)
	{
		return bots[index];
	}

	return nullptr;
}

/** Returns the number of bots.
*/
int Table::getNumOfBots() const
{
	return this->numOfBots;
}

/** Returns the amount of pot in game (on table).
*/
int Table::getPot() const
{
	int potSum = 0;
	for (int i = 0; i < this->numOfBots; ++i)
	{
		potSum += bots[i]->getPot();
	}

	return potSum;
}

/** Sits a bot to the table.
*/
void Table::sit(BotInfo* bot)
{
	// bots array already allocated in costructor
	this->bots[this->numOfBots++] = bot;
}