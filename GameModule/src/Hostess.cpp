#include "stdafx.h"
#include "Hostess.h"
#include "BroadcastMessage.h"

/** Inherited method to handle incoming broadcast messages.
*/
void Hostess::receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data)
{
	if (msg == BroadcastMessage::ALLINED)
	{
		int potPutIn = data[1];
		if (this->callAmount < potPutIn) // raise happened
		{
			this->handleRaise(potPutIn - this->callAmount);
		}
	}
	else if (msg == BroadcastMessage::BLINDSRAISED)
	{
		if (nextBlindShiftDeadlineIndex < this->rules->getNumOfBlinds() - 2)
		{
			// -1 because numOfBlinds = numOfBlindShift + 1; one more -1, bacause we don't want to index over
			++nextBlindShiftDeadlineIndex;
		}
	}
	else if (msg == BroadcastMessage::PREFLOP)
	{
		this->numberOfRaisesSoFar = 0;

		// whether bettingsystem, at preflop and flop the minRaise is the same
		// at turn fixlimit changes (others not)
		// during the betting potlimit changes (others not)
		this->minRaise = this->getBigBlindAtRound(this->round);
	}
	else if (msg == BroadcastMessage::FLOP)
	{
		this->numberOfRaisesSoFar = 0;

		if (this->rules->getBettingSystem() == BettingSystem::POTLIMIT)
		{
			// if potlimit, the minRaise may change during a betting round (there we reset it)
			this->minRaise = this->getBigBlindAtRound(this->round);
		}
	}
	else if (msg == BroadcastMessage::FOLDED)
	{
		int botIndex = this->getBotIndexByID(data[0]);
		this->botsInRound[botIndex] = false;
	}
	else if (msg == BroadcastMessage::LEFTGAME)
	{
		int botIndex = this->getBotIndexByID(data[0]);
		this->botsInGame[botIndex] = false;
		this->botsInRound[botIndex] = false;
	}
	else if (msg == BroadcastMessage::RAISED)
	{
		this->handleRaise(data[1]);
	}
	else if (msg == BroadcastMessage::ROUNDSTARTED)
	{
		// update botsInRound
		for (int i = 0; i < this->numOfBots; ++i)
		{
			this->botsInRound[i] = this->botsInGame[i];
		}

		this->round = data[0];
		this->callAmount = 0;
	}
	else if (msg == BroadcastMessage::TURN)
	{
		this->numberOfRaisesSoFar = 0;

		BettingSystem bs = this->rules->getBettingSystem();
		if (bs == BettingSystem::FIXLIMIT)
		{
			// if fixlimit minRaise doubles
			this->minRaise = 2 * this->minRaise;
		}
		else if (bs == BettingSystem::POTLIMIT)
		{
			// if potlimit, the minRaise may change during a betting round (there we reset it)
			this->minRaise = this->getBigBlindAtRound(this->round);
		}
	}
	else if (msg == BroadcastMessage::RIVER)
	{
		this->numberOfRaisesSoFar = 0;

		if (this->rules->getBettingSystem() == BettingSystem::POTLIMIT)
		{
			// if potlimit, the minRaise may change during a betting round (there we reset it)
			this->minRaise = this->getBigBlindAtRound(this->round);
		}
	}
}

/** Fills botsByID with bot reference and id pairs, fills botsInGame and botsInRound.
*/
void Hostess::fillBotsData()
{
	const BotInfo* tmpBot;
	
	for (int i = 0; i < this->numOfBots; ++i)
	{
		tmpBot = this->table->getBotByIndex(i);
		
		// insert bot into map
		this->botsByID.insert(std::pair<int, const BotInfo*>(tmpBot->getID(), tmpBot));

		// set botsInGame and botsInRound
		this->botsInGame[i] = true;
		this->botsInRound[i] = false;
	}
}

/** Do the needed changes to attibrutes, when raise happens.
 *	Sets numberOfRaisesSoFar, callAmount, minRaise.
*/
void Hostess::handleRaise(int raiseAmount)
{
	// number of raises so far
	++this->numberOfRaisesSoFar;

	// call amount
	this->callAmount = this->callAmount + raiseAmount;

	// minimum amount of raise (only at potlimit)
	if (this->rules->getBettingSystem() == BettingSystem::POTLIMIT)
	{
		this->minRaise = raiseAmount;
	}
}

/** Returns the maximum of the pots on table (by AIs).
*/
int Hostess::getCallAmount() const
{
	return this->callAmount;
}

/** Returns the minimum amount of raise (not containing the call) to put in.
*/
int Hostess::getMinRaise() const
{
	return this->minRaise;
}

/** Returns big blind at a specific (present/past/future) round.
*/
int Hostess::getBigBlindAtRound(int round) const
{
	int numOfShifts = this->rules->getNumOfBlinds() - 1; // 1 less than numOfBlinds
	int i;
	for (i = 0; i < numOfShifts; ++i)
	{
		if (round < this->rules->getBlindShiftDeadline(i))
		{
			return this->rules->getBigBlind(i); // draw it to see, it's working
		}
	}

	return this->rules->getBigBlind(i); // if there's no deadline ahead of the round, than it's the last of the blinds
}

/** Returns the next time occurence of a blind shift.
*/
int Hostess::getNextBlindShiftDeadline() const
{
	return this->rules->getBlindShiftDeadline(nextBlindShiftDeadlineIndex);
}

/** Returns small blind at a specific (present/past/future) round.
*/
int Hostess::getSmallBlindAtRound(int round) const
{
	int numOfShifts = this->rules->getNumOfBlinds() - 1; // 1 less than numOfBlinds
	int i;
	for (i = 0; i < numOfShifts; ++i)
	{
		if (round < this->rules->getBlindShiftDeadline(i))
		{
			return this->rules->getSmallBlind(i); // draw it to see, it's working
		}
	}

	return this->rules->getSmallBlind(i); // if there's no deadline ahead of the round, than it's the last of the blinds
}

/** Finds a bot's reference by the given id.
*/
const BotInfo* Hostess::getBotByID(int botID) const
{
	std::map<int, const BotInfo*>::const_iterator it = this->botsByID.find(botID);
	
	if (it == this->botsByID.cend()) // haven't found anything
	{
		return 0;
	}

	return it->second;
}

/** Returns the id of a bot by the given index.
*/
int Hostess::getBotIDByIndex(int index) const
{
	return this->table->getBotByIndex(index % this->numOfBots)->getID();
}

/** Returns AI's index by id (index: from 0 to n-1 same order at table; n: number of bots).
*/
int Hostess::getBotIndexByID(int botID) const
{
	for (int i = 0; i < this->numOfBots; ++i)
	{
		if (botID == this->table->getBotByIndex(i)->getID())
		{
			return i;
		}
	}

	return -1;
}

/** Returns the current round.
*/
int Hostess::getCurrentRound() const
{
	return this->round;
}

/** Returns the nth. bot to the right at table (the bots before).
*/
int Hostess::getBotIDToTheRight(int fromID, int nth, bool onlyInGame, bool onlyInRound) const
{
	int fromIndex = this->getBotIndexByID(fromID);
	bool isAny;

	// check if there's any in game
	if (onlyInGame)
	{
		isAny = false;
		for (int i = 0; i < this->numOfBots; ++i)
		{
			if (this->botsInGame[i])
			{
				isAny = true;
				break;
			}
		}

		if (!isAny)
		{
			return 0;
		}
	}

	// check if there's any in round
	if (onlyInRound)
	{
		isAny = false;
		for (int i = 0; i < this->numOfBots; ++i)
		{
			if (this->botsInRound[i])
			{
				isAny = true;
				break;
			}
		}

		if (!isAny)
		{
			return 0;
		}
	}

	int index;
	if (onlyInGame || onlyInRound)
	{
		index = fromIndex - 1;
		int activePlayersPassed = 0;
		while (activePlayersPassed < nth)
		{
			if ( (this->botsInGame[index % this->numOfBots] == onlyInGame || !onlyInGame)
				&& (this->botsInRound[index % this->numOfBots] == onlyInRound || !onlyInRound) )
			{
				// false means nothing..
				++activePlayersPassed;
				--index;
			}
		}
	}
	else
	{
		index = fromIndex - nth;
	}

	return index % this->numOfBots;
}

/** Returns the nth. bot to the left at table (the bots after).
*/
int Hostess::getBotIDToTheLeft(int fromID, int nth, bool onlyInGame, bool onlyInRound) const
{
	int fromIndex = this->getBotIndexByID(fromID);
	bool isAny;

	// check if there's any in game
	if (onlyInGame)
	{
		isAny = false;
		for (int i = 0; i < this->numOfBots; ++i)
		{
			if (this->botsInGame[i])
			{
				isAny = true;
				break;
			}
		}

		if (!isAny)
		{
			return 0;
		}
	}

	// check if there's any in round
	if (onlyInRound)
	{
		isAny = false;
		for (int i = 0; i < this->numOfBots; ++i)
		{
			if (this->botsInRound[i])
			{
				isAny = true;
				break;
			}
		}

		if (!isAny)
		{
			return 0;
		}
	}

	int index;
	if (onlyInGame || onlyInRound)
	{
		index = fromIndex + 1;
		int activePlayersPassed = 0;
		while (activePlayersPassed < nth)
		{
			if ( (this->botsInGame[index % this->numOfBots] == onlyInGame || !onlyInGame)
				&& (this->botsInRound[index % this->numOfBots] == onlyInRound || !onlyInRound) )
			{
				// false means nothing..
				++activePlayersPassed;
				++index;
			}
		}
	}
	else
	{
		index = fromIndex + nth;
	}

	return index % this->numOfBots;
}

/** Returns the number of bots.
*/
int Hostess::getNumOfBots(bool onlyInGame, bool onlyInRound) const
{
	int num = 0;
	for (int i = 0; i < this->numOfBots; ++i)
	{
		if ( (this->botsInGame[i] == onlyInGame || !onlyInGame)
			&& (this->botsInRound[i] == onlyInRound || !onlyInRound) )
		{
			++num;
		}
	}

	return num;
}

/** Returns the number of raises left in a round that is possible.
*/
int Hostess::getNumberOfRaisesLeft() const
{
	if (this->rules->getMaxNumOfRaises() > 0) // zero means no limit for number of raises
	{
		return this->rules->getMaxNumOfRaises() - this->numberOfRaisesSoFar;
	}

	return 1; // 1 is enough to let a player raise if there's no limit
}