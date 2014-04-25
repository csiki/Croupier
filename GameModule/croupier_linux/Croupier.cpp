#include "stdafx.h"
#include "Croupier.h"
#include "BroadcastMessage.h"

/** Inherited method to handle incoming broadcast messages.
*/
void Croupier::receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data)
{
	switch (msg)
	{
		case RAISED:
			this->lastBotRaisedIndex = this->findBotIndexByID(data[0]); // for betting rounds
			break;
	}
}

/** Comparator for sorting bots by their amount of pot.
*/
bool Croupier::botComparatorByPot(int botIndex1, int botIndex2)
{
	return this->bots[botIndex1]->getPot() < this->bots[botIndex2]->getPot();
}

/** Burn a card.
*/
void Croupier::burn(const Card& c)
{
	Logger::Log(this, Severity::NOTIFICATION, "burn ", c);
	this->burnt.push_back(c);
}

/** Puts burnt cards, cards from the table, and hands back to deck.
*/
void Croupier::collectCards()
{
	// log
	Logger::Log(this, Severity::NOTIFICATION, "collectCards");

	// burnt cards
	while(!this->burnt.empty())
	{
	  this->deck.push(burnt.back());
	  burnt.pop_back();
	}

	// cards from table
	while(this->table->getNumOfCards() > 0)
	{
	  this->deck.push(this->table->rmCard());
	}

	// hands
	for (size_t i = 0; i < this->numOfBots; ++i)
	{
		if (this->bots[i]->isInGame())
		{
			// if a bot is in game, then it must have cards
			this->deck.push(this->bots[i]->takeHand());
			this->deck.push(this->bots[i]->takeHand());
		}
	}
}

/** Starts a bet round (from currentBotIndex).
*/
void Croupier::betRound()
{
	// log
	Logger::Log(this, Severity::NOTIFICATION, "betRound");

	this->lastBotRaisedIndex = this->currentBotIndex; // if noone raises the bet round ends before currentBotIndex
	// further lastBotRaisedIndex is set by receiveBroadcast/RAISED

	int botIndex = this->currentBotIndex; // betting starts from currentBotIndex
	do
	{
		if (this->bots[botIndex]->isInRound())
		{
			this->bots[botIndex]->step();
		}

		botIndex = (botIndex + 1) % this->numOfBots;

	} while (botIndex != this->lastBotRaisedIndex && this->numOfBotsInRound() > 1);
}

/** Deals cards to bots.
*/
void Croupier::dealing()
{
	// log
	Logger::Log(this, Severity::NOTIFICATION, "dealing");

	// shuffle deck
	this->deck.shuffle();

	// first card
	for (size_t i = 0; i < this->numOfBots; ++i)
	{
		if (this->bots[i]->isInRound())
		{
			this->bots[i]->receiveCard(this->deck.pop());
		}
	}

	// second card
	for (size_t i = 0; i < this->numOfBots; ++i)
	{
		if (this->bots[i]->isInRound())
		{
			this->bots[i]->receiveCard(this->deck.pop());
		}
	}
}

/** Find index of the next active bot.
*/
int Croupier::nextActiveBot(int from) const
{
	if (from == -1) // default value
	{
		from = this->currentBotIndex;
	}

	int offset = 1;
	while (!this->bots[(from + offset) % this->numOfBots]->isInRound())
	{
		++offset; // iterate through bots till we found an active
	}

	return (from + offset) % this->numOfBots;
}

/** Forces small and big blind, deals cards, broadcasts, starts a bet round.
*/
void Croupier::preflop()
{
	// force blinds
	int botIndex = this->currentDealerIndex; // dealer

	botIndex = this->nextActiveBot(botIndex); // small blind
	this->bots[botIndex]->forceBlind(this->rules->getSmallBlind(this->currentBlindIndex));

	botIndex = this->nextActiveBot(botIndex); // big blind
	this->bots[botIndex]->forceBlind(this->rules->getBigBlind(this->currentBlindIndex));

	// deal cards
	this->dealing();

    // log
	Logger::Log(this, Severity::NOTIFICATION, "preflop");

	// broadcast
	this->broadcast(BroadcastMessage::PREFLOP, 0, 0);

	// bet round
	this->currentBotIndex = this->nextActiveBot(botIndex); // the one after bigblind, betRound() uses currentBotIndex
	this->betRound();
}

/** Burns, puts 3 cards on table, broadcasts, starts a bet round.
*/
void Croupier::flop()
{
	// burn
	this->burn(this->deck.pop());

	// put 3 cards on table
	Card c1 = this->deck.pop();
	Card c2 = this->deck.pop();
	Card c3 = this->deck.pop();
	this->table->addCard(c1);
	this->table->addCard(c2);
	this->table->addCard(c3);

	Logger::Log(this, Severity::NOTIFICATION, "flop ", c1, ',', c2, ',', c3);

	// broadcast
	this->broadcast(BroadcastMessage::FLOP, 0, 0);

	// start bet round
	this->currentBotIndex = this->nextActiveBot(this->currentDealerIndex); // the one after the dealer (small blind)
	this->betRound();
}

/** Burns, puts one more card on table, broadcasts, starts a bet round (turn).
*/
void Croupier::turn()
{
	// burn
	this->burn(this->deck.pop());

	// put a card on table
	Card c = this->deck.pop();
	this->table->addCard(c);

	Logger::Log(this, Severity::NOTIFICATION, "turn ", c);

	// broadcast
	this->broadcast(BroadcastMessage::TURN, 0, 0);

	// start bet round
	this->currentBotIndex = this->nextActiveBot(this->currentDealerIndex); // the one after the dealer (small blind)
	this->betRound();
}

/** Burns, puts one more card on table, broadcasts, starts a bet round (river).
*/
void Croupier::river()
{
	// burn
	this->burn(this->deck.pop());

	// put a card on table
	Card c = this->deck.pop();
	this->table->addCard(c);

	Logger::Log(this, Severity::NOTIFICATION, "river ", c);

	// broadcast
	this->broadcast(BroadcastMessage::RIVER, 0, 0);

	// start bet round
	this->currentBotIndex = this->nextActiveBot(this->currentDealerIndex); // the one after the dealer (small blind)
	this->betRound();
}

/** Direct bots to reveal cards, broadcasts.
*/
void Croupier::showdown()
{
	// log
	Logger::Log(this, Severity::NOTIFICATION, "showdown");

	// reveal cards
	for (size_t i = 0; i < this->numOfBots; ++i)
	{
		if (this->bots[i]->isInRound())
		{
			this->bots[i]->revealCards();
		}
	}

	// broadcast
	this->broadcast(BroadcastMessage::SHOWDOWN, 0, 0);
}

/** Hand out pot for one winner.
*/
void Croupier::handOutPot(int winnerIndex)
{
	Logger::Log(this, Severity::NOTIFICATION, "handOutPot ", 1, ',', this->bots[winnerIndex]->getName());

	BotHandler* winner = this->bots[winnerIndex];
	int winnersPot = winner->getPot();

	for (size_t i = 0; i < this->numOfBots; ++i)
	{
		// give the winner at least the amount of pot he got
		winner->receiveChips(this->bots[i]->takePot(winnersPot));

		// if there's more pot at the loosers (or the winner), then they should take it
		this->bots[i]->receiveChips(this->bots[i]->takePot());
	}
}

/** Hand out pot for more than one winners.
*/
void Croupier::handOutPot(int numOfWinners, int* winnersIndex)
{
	// log
  std::ostringstream os;
  os << "handOutPot " << numOfWinners;
	for (size_t i = 0; i < numOfWinners; ++i)
	{
		os << ',' << this->bots[winnersIndex[i]]->getName();
	}
	Logger::Log(this, Severity::NOTIFICATION, os.str());

	// fill winners to easily find out if a given index represents a winning or losing bot
	bool* areWinners = new bool[this->numOfBots];
	for (size_t i = 0; i < this->numOfBots; ++i)
	{
		areWinners[i] = false;
		for (size_t j = 0; j < numOfWinners; ++j)
		{
			if (winnersIndex[j] == i)
			{
				areWinners[i] = true;
				break;
			}
		}
	}

	// sort winners compared by pots
	std::sort(winnersIndex, winnersIndex + numOfWinners,
		std::bind(&Croupier::botComparatorByPot, this, std::placeholders::_1, std::placeholders::_2));

	// split pot
	int amountToTake, amountAlreadyTaken, divider, loosersOriginalPot;
	BotHandler *winnerBot, *looserBot;

	for (size_t l = 0; l < this->numOfBots; ++l) // iterate through...
	{
		amountAlreadyTaken = 0;
		if (!areWinners[l]) // ... loosers
		{
			looserBot = this->bots[l];
			loosersOriginalPot = looserBot->getPot();
			divider = numOfWinners;

			for (size_t w = 0; w < numOfWinners; ++w) // iterate through winners
			{
				winnerBot = this->bots[winnersIndex[w]];
				if (loosersOriginalPot < winnerBot->getPot())
				{
					// Looser got less, so we get his amount of pot, divide it
					// and take it from it, than give it to the winner.
					amountToTake = looserBot->getPot() / divider;
				}
				else
				{
					// Winner got less, so we get his amount of pot (minus already taken from looser), divide it
					// and take it from the looser, than give it to the winner.
					amountToTake = (winnerBot->getPot() - amountAlreadyTaken) / divider;
				}

				// give looser's pot to winner
				winnerBot->receiveChips( looserBot->takePot(amountToTake) );

				// decrease divider
				--divider;
			}
		}
	}

	// take the rest of the pot to chips / bot
	for (size_t i = 0; i < this->numOfBots; ++i)
	{
		this->bots[i]->receiveChips(this->bots[i]->takePot());
	}
}

/** Changes blinds if necessary.
*/
void Croupier::refreshBlinds()
{
	if (this->rules->getBlindShiftDeadline(this->nextBlindShiftAtIndex) == this->round)
	{
		// change blinds
		++this->currentBlindIndex;

		// refresh next blind shift index
		if (this->nextBlindShiftAtIndex < this->rules->getNumOfBlinds() - 2)
		{
			// -1 because (blindsNum == blindShiftNum + 1), and -1 because increasing index
			++this->nextBlindShiftAtIndex;
		}

		Logger::Log(this, Severity::NOTIFICATION, "refreshBlinds ",
		    this->rules->getSmallBlind(this->currentBlindIndex), ',',
		    this->rules->getBigBlind(this->currentBlindIndex));

		// broadcast blind raise
		int* msgdata = new int[2];
		msgdata[0] = this->rules->getSmallBlind(this->currentBlindIndex);
		msgdata[1] = this->rules->getBigBlind(this->currentBlindIndex);
		this->broadcast(BroadcastMessage::BLINDSRAISED, 2, msgdata);
		delete [] msgdata;
	}
}

/** Determine one or multiple winners.
*/
void Croupier::determineWinners(int& numOfWinners, int** winnersIndex)
{
	// count bots in round (here we don't use numOfBotsInRound() method because we need tmpIfOneWinnerIndex)
	int numOfBotsInRound = 0;
	int tmpIfOneWinnerIndex; // if one bot stayed, it stores its index, so don't have to look up again later
	for (size_t i = 0; i < this->numOfBots; ++i)
	{
		if (this->bots[i]->isInRound())
		{
			++numOfBotsInRound;
			tmpIfOneWinnerIndex = i;
		}
	}

	// if only one bot stayed, all others folded
	if (numOfBotsInRound == 1)
	{
		numOfWinners = 1;
		*winnersIndex = new int[1];
		(*winnersIndex)[0] = tmpIfOneWinnerIndex;

		return;
	}

	// if showdown went down, and multiple bots stayed in round
	std::vector<Card> tmpHand(7);

	// tmpHand array first five items are the cards on table
	for (size_t i = 0; i < 5; ++i)
	{
	  tmpHand[i] = this->table->getCard(i);
	}

	// eval all hands in game, find hands with the (same) highest rank, compare them
	// by iterating through inRound bots only once
	std::list<int> winners;

	unsigned bestHandValue = 0, tmpHandValue;
	for (size_t i = 0; i < this->numOfBots; ++i)
  {
    if (this->bots[i]->isInRound())
    {
      // tmpHand array last 2 elements are the ones in the actual player's hand
      tmpHand[5] = this->bots[i]->checkCard(0);
      tmpHand[6] = this->bots[i]->checkCard(1);

      tmpHandValue = HandEvaluator::evalHandValue(tmpHand);
      if(tmpHandValue > bestHandValue)
      {
        bestHandValue = tmpHandValue;
        winners.clear();
        winners.push_back(i);
      }
      else if(tmpHandValue == bestHandValue)
      {
        winners.push_back(i);
      }
    }
  }

	// fill winnersIndex, declare numOfWinners
	numOfWinners = winners.size();
	*winnersIndex = new int[numOfWinners];

	int i = 0;
	for (std::list<int>::iterator it = winners.begin(); it != winners.end(); ++it)
	{
		(*winnersIndex)[i++] = *it;
	}

	// broadcast
	this->broadcast(BroadcastMessage::ROUNDWINNERS, numOfWinners, *winnersIndex);

	// log
	std::ostringstream os;
	os << "roundWinners " << numOfWinners;
	for (size_t i = 0; i < numOfWinners; ++i)
	{
		os << ',' << this->bots[((*winnersIndex)[i])]->getName();
	}
	Logger::Log(this, Severity::NOTIFICATION, os.str());
}

/** Finds a bot's index in bots array by the given id.
*/
int Croupier::findBotIndexByID(int botID) const
{
	for (size_t i = 0; i < this->numOfBots; ++i)
	{
		if (this->bots[i]->getID() == botID)
		{
			return i;
		}
	}

	return -1;
}

/** Direct a game.
 *	GameOwner calls it, signalling the start of the game.
*/
void Croupier::letsPoker()
{
	// log
	Logger::Log(this, Severity::NOTIFICATION, "letsPoker");

	this->round = 1; // the very first round, not 0

	while (this->canStartNewRound())
	{
		// refresh blinds (if needed)
		this->refreshBlinds();

		// log round started
		Logger::Log(this, Severity::NOTIFICATION, "roundStarted ", this->round);

		// broadcast round started
		this->broadcast(BroadcastMessage::ROUNDSTARTED, 1, &this->round);

		// push the dealer button to the next active bot
		this->bots[this->currentDealerIndex]->rmDealerButton();
		Logger::Log(this, Severity::NOTIFICATION, "rmDealerButton ", this->bots[this->currentDealerIndex]->getID());

		// add dealer button
		this->currentDealerIndex = this->nextActiveBot(this->currentDealerIndex);
		this->bots[this->currentDealerIndex]->addDealerButton();
		Logger::Log(this, Severity::NOTIFICATION, "addDealerButton ", this->bots[this->currentDealerIndex]->getID());

		// log and broadcast rebuy deadline reach if reached
		if (this->round == this->rules->getRebuyDeadline())
		{
			Logger::Log(this, Severity::NOTIFICATION, "rebuyDeadlineReached");
			this->broadcast(BroadcastMessage::REBUYDEADLINEREACHED, 0, 0);
		}

		// preflop
		this->preflop();

		// flop
		if (this->canRoundGoOn())
		{
			this->flop();

			// turn
			if (this->canRoundGoOn())
			{
				this->turn();

				// river
				if (this->canRoundGoOn())
				{
					this->river();

					// showdown
					if (this->canRoundGoOn())
					{
						this->showdown();
					}
				}
			}
		}

		// determine round winners
		int numOfWinners = 0;
		int* winnersIndex;
		this->determineWinners(numOfWinners, &winnersIndex);

		if (numOfWinners == 1) // only one winner, obvious handout
		{
			this->handOutPot(winnersIndex[0]);
		}
		else // multiple winners
		{
			this->handOutPot(numOfWinners, winnersIndex);
		}

		// collect cards
		this->collectCards();

		// send rebuyOrLeave() to bots out of chips (if rebuy is possible)
		// botmanager runs canRebuy() before forward the message to bot
		if (this->rules->getNumOfRebuysAllowed() > 0)
		{
			for (size_t i = 0; i < this->numOfBots; ++i)
			{
				if (this->bots[i]->getChips() == 0)
				{
					Logger::Log(this, Severity::NOTIFICATION, "rebuyOrLeave ", this->bots[i]->getID());
					this->bots[i]->rebuyOrLeave();
				}
			}
		}

		// send leave() to those bots, who still has 0 chips
		for (size_t i = 0; i < this->numOfBots; ++i)
		{
			if (this->bots[i]->getChips() == 0)
			{
				Logger::Log(this, Severity::NOTIFICATION, "leave ", this->bots[i]->getID());
				this->bots[i]->leave();
			}
		}

		// log and broadcast end of round
		Logger::Log(this, Severity::NOTIFICATION, "roundEnded");
		this->broadcast(BroadcastMessage::ROUNDENDED, 0, 0);

		++this->round;
	}

	// declare winner
	int winnerIndex;
	for (size_t i = 0; i < this->numOfBots; ++i)
	{
		if (this->bots[i]->isInGame())
		{
			winnerIndex = i;
			break;
		}
	}

	// log and broadcast game winner
	int winnerID = this->bots[winnerIndex]->getID();
	Logger::Log(this, Severity::NOTIFICATION, "gameWinner ", winnerID);
	this->broadcast(BroadcastMessage::GAMEWINNER, 1, &winnerID);
}

/** Stores a bot handler at the given index.
*/
void Croupier::provideBotHandler(int index, BotHandler* bh)
{
	this->bots[index] = bh;
}

/** Kicks a bot (inGame := false).
*/
void Croupier::kickBot(int botID)
{
	Logger::Log(this, Severity::NOTIFICATION, "kickBot ", botID);
	int botIndex = this->findBotIndexByID(botID);
	this->bots[botIndex]->leave();
}

/** Returns if a new round can be started (or the game ended).
*/
bool Croupier::canStartNewRound() const
{
	// count the number of bots inGame
	int numOfBotsInGame = 0;
	for (size_t i = 0; i < this->numOfBots; ++i)
	{
		if (this->bots[i]->isInGame())
		{
			++numOfBotsInGame;
		}
	}

	return numOfBotsInGame >= 2;
}

/** Returns if the round can go on (or ended for example because of one inRound player left).
*/
bool Croupier::canRoundGoOn() const
{
	return this->numOfBotsInRound() >= 2;
}

/** Returns the number of bots in round (have not folded).
*/
size_t Croupier::numOfBotsInRound() const
{
	int numOfBotsInRound = 0;
	for (size_t i = 0; i < this->numOfBots; ++i)
	{
		if (this->bots[i]->isInRound())
		{
			++numOfBotsInRound;
		}
	}

	return numOfBotsInRound;
}
