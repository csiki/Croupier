#include "stdafx.h"
#include "BotHandler.h"

/**	Adds the dealer button to the AI.
*/
void BotHandler::addDealerButton()
{
	this->dealer = true;
}

/**	Removes the dealer button of the AI.
*/
void BotHandler::rmDealerButton()
{
	this->dealer = false;
}

/**	Command the bot for a blind.
*/
void BotHandler::forceBlind(int blind)
{
	if (this->chips < blind)
	{
		this->pot = this->chips;
		this->chips = 0;
	}
	else
	{
		this->pot = blind;
		this->chips -= blind;
	}
}

/**	Adds a card to the AI's hand.
*/
void BotHandler::receiveCard(Card* c)
{
	if (this->numOfCardsInHand < 2)
	{
		this->hand[this->numOfCardsInHand] = c;
		++this->numOfCardsInHand;
	}
}

/**	Adds more chips to AI's existing chips.
*/
void BotHandler::recieveChips(int chipsAmount)
{
	this->chips += chipsAmount;
}

/**	Command AI to reveal its cards.
*/
void BotHandler::revealCards()
{
	this->cardsRevealed = true;
}

/**	Removes a card from AI's hand.
*/
Card* BotHandler::takeHand(int cardIndex)
{
	Card* temp = this->hand[cardIndex];
	this->hand[cardIndex] = 0;
	return temp;
}

/**	Takes all the pot of the AI, and returns its amount.
*/
int BotHandler::takePot()
{
	int temp = this->pot;
	this->pot = 0;
	return temp;
}

