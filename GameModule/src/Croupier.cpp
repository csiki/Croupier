#include "stdafx.h"
#include "Croupier.h"
#include "BroadcastMessage.h"

/** Inherited method to handle incoming broadcast messages.
*/
void Croupier::receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data)
{
	switch (msg)
	{
		case LEFTGAME:
			this->kickBot(data[0]); // if a bot decides to leave, kick it
			break;
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

/** Starts a bet round (from currentBotIndex).
*/
void Croupier::betRound()
{
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
	} while (botIndex != this->lastBotRaisedIndex); // TODO it depends on the poker game type
}

/** Deals cards to bots.
*/
void Croupier::dealing()
{
	for (int i = 0; i < this->numOfBots; ++i)
	{
		if (this->bots[i]->isInRound())
		{
			this->bots[i]->receiveCard(this->deck->pop());
		}
	}

	for (int i = 0; i < this->numOfBots; ++i) // twice
	{
		if (this->bots[i]->isInRound())
		{
			this->bots[i]->receiveCard(this->deck->pop());
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

		if (offset > numOfBots)
		{
			throw "No active bots found!"; // TEST törölni késõbb
		}
	}

	return (from + offset) % this->numOfBots;
}

/** Forces small and big blind, deals cards, broadcasts, starts a bet round.
*/
void Croupier::preflop()
{
	// force blinds
	int botIndex = this->findDealerBotIndex(); // dealer
	
	botIndex = this->nextActiveBot(botIndex); // small blind
	this->bots[botIndex]->forceBlind(this->currentSmallBlind);
	
	botIndex = this->nextActiveBot(botIndex); // big blind
	this->bots[botIndex]->forceBlind(this->currentBigBlind);

	// deal cards
	this->dealing();

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
	this->burn(this->deck->pop());

	// put 3 cards on table
	this->table->addCard(this->deck->pop());
	this->table->addCard(this->deck->pop());
	this->table->addCard(this->deck->pop());

	// broadcast
	this->broadcast(BroadcastMessage::FLOP, 0, 0);

	// start bet round
	this->currentBotIndex = this->nextActiveBot(this->findDealerBotIndex()); // the one after the dealer (small blind)
	this->betRound();
}

/** Burns, puts one more card on table, broadcasts, starts a bet round (turn).
*/
void Croupier::turn()
{
	// burn
	this->burn(this->deck->pop());

	// put a card on table
	this->table->addCard(this->deck->pop());

	// broadcast
	this->broadcast(BroadcastMessage::TURN, 0, 0);

	// start bet round
	this->currentBotIndex = this->nextActiveBot(this->findDealerBotIndex()); // the one after the dealer (small blind)
	this->betRound();
}

/** Burns, puts one more card on table, broadcasts, starts a bet round (river).
*/
void Croupier::river()
{
	// burn
	this->burn(this->deck->pop());

	// put a card on table
	this->table->addCard(this->deck->pop());

	// broadcast
	this->broadcast(BroadcastMessage::RIVER, 0, 0);

	// start bet round
	this->currentBotIndex = this->nextActiveBot(this->findDealerBotIndex()); // the one after the dealer (small blind)
	this->betRound();
}

/** Direct bots to reveal cards, broadcasts.
*/
void Croupier::showdown()
{
	// TODO ? bots should have the possibility to muck (fold before showing cards)
	
	// reveal cards
	for (int i = 0; i < this->numOfBots; ++i)
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
	BotHandler* winner = this->bots[winnerIndex];
	int winnersPot = winner->getPot();

	for (int i = 0; i < this->numOfBots; ++i)
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
	// TODO check if it works entirely properly !!!

	// fill winners to easily find out if a given index represents a winning or losing bot
	bool* areWinners = new bool[this->numOfBots];
	for (int i = 0; i < this->numOfBots; ++i)
	{
		areWinners[i] = false;
		for (int j = 0; j < numOfWinners; ++j)
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

	for (int l = 0; l < this->numOfBots; ++l) // iterate through...
	{
		amountAlreadyTaken = 0;
		if (!areWinners[l]) // ... loosers
		{
			looserBot = this->bots[l];
			loosersOriginalPot = looserBot->getPot();
			divider = numOfWinners;

			for (int w = 0; w < numOfWinners; ++w) // iterate through winners
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
	for (int i = 0; i < this->numOfBots; ++i)
	{
		this->bots[i]->receiveChips(this->bots[i]->takePot());
	}
}

void Croupier::refreshBlinds()
{
	
}

void Croupier::determineWinners(int numOfWinners, int* winnersIndex)
{
	// TODO broadcast!
}

/** Finds a bot's index in bots array by the given id.
*/
int Croupier::findBotIndexByID(int botID) const
{
	for (int i = 0; i < this->numOfBots; ++i)
	{
		if (this->bots[i]->getID() == botID)
		{
			return i;
		}
	}

	throw "No bot found"; // TEST

	return -1;
}

/** Finds the currently dealer bot's index.
*/
int Croupier::findDealerBotIndex() const
{
	for (int i = 0; i < this->numOfBots; ++i)
	{
		if (this->bots[i]->isDealer())
		{
			return i;
		}
	}

	throw "No bot found"; // TEST

	return -1;
}

void Croupier::letsPoker()
{

}

/** Returns the round when a specified bot is kicked or left the game.
*/
int Croupier::getKickAtRound(int botIndex) const
{
	return this->kicksAtRound[botIndex];
}

/** Returns bot id by indexing in croupiers array.
*/
int Croupier::getBotIDByIndex(int botIndex) const
{
	return this->bots[botIndex]->getID();
}

/** Stores bot handlers.
*/
void Croupier::provideBotHandlers(int numOfBots, BotHandler** bhs)
{
	this->numOfBots = numOfBots;
	this->bots = bhs;
	this->kicksAtRound = new int[numOfBots];
}

/** Stores a reference of the table
*/
void Croupier::provideTable(Table* table)
{
	this->table = table;
}

/** Kicks a bot (inGame := false).
*/
void Croupier::kickBot(int botID)
{
	int botIndex = this->findBotIndexByID(botID);
	
	this->bots[botIndex]->quit();
	this->kicksAtRound[botIndex] = this->round;
}
