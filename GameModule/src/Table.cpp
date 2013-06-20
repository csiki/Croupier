#include "stdafx.h"
#include "Table.h"

void Table::addCard(Card* c)
{
	this->cards[++this->numOfCards] = c;
}

int Table::getNumOfCards() const
{
	return this->numOfCards;
}

Card* Table::rmCard(int cardIndex)
{
	//TEST out of range
	Card* card = cards[cardIndex];
	for (int i = cardIndex; i < this->numOfCards-1; i++)
	{
		cards[i] = cards[i+1];
	}
	--numOfCards;
	return card;
}

const Card* Table::getCard(int index) const
{
	//TEST out of range
	return cards[index];
}

const BotInfo* Table::getBotByIndex(int index) const
{
	//TEST out of range
	return bots[index];
}

int Table::getNumOfBots() const
{
	return numOfBots;
}

int Table::getPot() const
{
	int potSum = 0;
	for (int i = 0; i < numOfBots; i++)
	{
		potSum += bots[i]->getPot();
	}
	return potSum;
}

void Table::sit(BotInfo* bot)
{
	BotInfo** newArray = new BotInfo*[this->numOfBots+1];
	//copy old array
	for (int i = 0; i < this->numOfBots; i++)
	{
		newArray[i] = bots[i];
	}
	//insert new BotInfo*
	++this->numOfBots;
	newArray[this->numOfBots-1] = bot;
	delete[] bots;
	bots = newArray;
}

void Table::getUp(BotInfo* bot)
{
	//TEST bot exits
	int botIndex = 0;
	BotInfo** newArray = new BotInfo*[this->numOfBots-1];
	for (int i = 0; i < botIndex; i++) //copy before
	{
		newArray[i] = bots[i];
	}

	if (botIndex != (numOfBots - 1))
	{
		for (int i = botIndex; i < numOfBots-1; i++) //copy after
		{
			newArray[i] = bots[i+1];
		}
	}
	--this->numOfBots;
	delete[] bots;
	bots = newArray;
}

