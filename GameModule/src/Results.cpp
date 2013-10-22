#include "stdafx.h"
#include "Results.h"

/** Adds a bot's result to results.
*/
void Results::addResult(int id, int playerID, int credit, int kickAtRound)
{
	this->botIDs[this->numOfBots] = id;
	this->playerIDs[this->numOfBots] = playerID;
	this->credits[this->numOfBots] = credit;
	this->kicksAtRound[this->numOfBots] = kickAtRound;
	++this->numOfBots;
}

/** Gets the number of bots already added.
*/
int Results::getNumOfPlayers() const
{
	return this->numOfBots;
}

/** Gets a bot's id.
*/
int Results::getBotID(int index) const
{
	return this->botIDs[index];
}

/** Gets the bot owner's id.
*/
int Results::getPlayerID(int index) const
{
	return this->playerIDs[index];
}

/** Gets a bot's credit.
*/
int Results::getCredit(int index) const
{
	return this->credits[index];
}

/** Gets a bot's number of round when it fell out.
*/
int Results::getKickAtRound(int index) const
{
	return this->kicksAtRound[index];
}