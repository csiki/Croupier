#include "stdafx.h"
#include "Table.h"

/** Adds a card to the ones on table (flop, turn, river).
*/
void Table::addCard(const Card& c)
{
  this->cards.push_back(c);
}

/** Returns the number of cards on table (flop, turn, river).
*/
int Table::getNumOfCards() const
{
	return this->cards.size();
}

/** Removes from table (if there's any flop, turn, river).
*/
Card Table::rmCard()
{
	if (!this->cards.empty())
	{
		Card c = this->cards.back();
		this->cards.pop_back();
		return c;
	}

	return Card::getNullCard();
}

/** Returns a card from table (flop, turn, river).
*/
Card Table::getCard(size_t index) const
{
	if (index < this->cards.size())
	{
		return this->cards[index];
	}

	return Card::getNullCard();
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
	for (size_t i = 0; i < this->numOfBots; ++i)
	{
		potSum += bots[i]->getPot();
	}

	return potSum;
}

/** Sits a bot to the table.
*/
void Table::sit(BotInfo* bot)
{
	this->bots[this->numOfBots++] = bot;
}
