#include "stdafx.h"
#include "BotHandler.h"

/**	Can see the bot's card. (returns pointer!)
*/
Card BotHandler::checkCard(size_t index)
{
    return this->hand[index];
}

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

	// broadcast the pot put in as raise
	int* msgdata = new int[2];
	msgdata[0] = this->getID();
	msgdata[1] = this->pot; // = raise amount
	this->broadcast(BroadcastMessage::RAISED, 2, msgdata);
	delete [] msgdata;

	// increase number of raises
	++this->numOfRaises;
}

/**	Adds a card to the AI's hand.
*/
void BotHandler::receiveCard(const Card& c)
{
	// log
	std::string msg = "receiveCard ";
	msg += c.toString();
	this->log(Severity::VERBOSE, msg);

	this->hand.push_back(c);
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

	// log cards (as information)
	std::string msg = "revealCards ";
	msg += this->lookAtHand(0).toString();
	msg += ",";
	msg += this->lookAtHand(1).toString();
	this->log(Severity::INFORMATION, msg);
}

/**	Removes a card from AI's hand.
*/
Card BotHandler::takeHand()
{
  Card tmp = this->hand.back();
  this->hand.pop_back();
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

