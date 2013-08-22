#include "stdafx.h"
#include "Rulz.h"

/** Returns the big blind by the given index.
*/
int Rulz::getBigBlind(int index) const
{
	if (index >= 0 && index < this->numOfBlinds)
	{
		return this->bigBlinds[index];
	}

	return false;
}

/** Returns the round of the index. blind shift.
*/
int Rulz::getBlindShiftDeadline(int index) const
{
	// numOfBlindShifts + 1 == numOfBlinds
	if (index >= 0 && index < this->numOfBlinds - 1)
	{
		return this->blindShiftDeadlines[index];
	}

	return false;
}

/** Returns the number of rebuys allowed for bots.
*/
int Rulz::getNumOfRebuysAllowed() const
{
	return this->numOfRebuysAllowed;
}

/** Returns the number of round when rebuy last time possible.
*/
int Rulz::getRebuyDeadline() const
{
	return this->rebuyDeadline;
}

/** Returns the small blind by the given index.
*/
int Rulz::getSmallBlind(int index) const
{
	if (index >= 0 && index < this->numOfBlinds)
	{
		return this->smallBlinds[index];
	}

	return false;
}

/** Returns if talk is allowed.
*/
bool Rulz::isTalkAllowed() const
{
	return this->talkAllowed;
}

/** Returns if expressing emotions is allowed.
*/
bool Rulz::isEmotionAllowed() const
{
	return this->emotionAllowed;
}

/** Returns if using permanent bot data is allowed.
*/
bool Rulz::isBotKnowledgeUseAllowed() const
{
	return this->knowledgeBaseAllowed;
}

/** Returns allowed bot calculation time in milisecs for the given language.
*/
int Rulz::getAllowedBotCalcTime(BotLanguage lang) const
{
	return this->allowedBotCalcTime.at(lang);
}

/** Returns the amount of chips all the bots started playing 1-by-1.
*/
int Rulz::getStartingChips() const
{
	return this->startingChips;
}

/** Returns number of different amount of blinds in game.
*/
int Rulz::getNumOfBlinds() const
{
	return this->numOfBlinds;
}

/** Returns the betting system of the game.
*/
BettingSystem Rulz::getBettingSystem() const
{
	return this->bs;
}