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
	if (this->hand[0] != 0)
	{
		this->hand[0] = c;
	}
	else if (this->hand[1] != 0)
	{
		this->hand[1] = c;
	}
}

/**	Adds more chips to AI's existing chips.
*/
void BotHandler::receiveChips(int chipsAmount)
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
	Card* tmp = this->hand[cardIndex];
	this->hand[cardIndex] = 0;
	return tmp;
}

/**	Takes all the pot of the AI, and returns its amount.
*/
int BotHandler::takePot(int amount)
{
	if (this->pot < amount || amount == -1)
	{
		// if ask more than pot or default -1, it gives all the pot
		amount = this->pot;
	}

	this->pot -= amount;
	return amount;
}

