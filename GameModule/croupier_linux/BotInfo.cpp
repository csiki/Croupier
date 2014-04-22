#include "stdafx.h"
#include "BotInfo.h"

/** Returns AI's amount of chips.
*/
extern "C" int BotInfo::getChips() const
{
	return this->chips;
}

/** Returns AI's pot in game.
*/
int BotInfo::getPot() const
{
	return this->pot;
}

/** Returns AI's current emotion.
*/
Emotion BotInfo::getEmotion() const
{
	return this->emotion;
}

/** Returns if AI's the dealer.
*/
bool BotInfo::isDealer() const
{
	return this->dealer;
}

/** Returns if AI's hand is revealed.
*/
bool BotInfo::isHandRevealed() const
{
	return this->cardsRevealed;
}

/** Returns if AI is in game (haven't lost all its chips).
*/
bool BotInfo::isInGame() const
{
	return this->inGame;
}

/** Returns if AI is in round (haven't folded).
*/
bool BotInfo::isInRound() const
{
	return this->inRound;
}

/** Checks a card of the AI's hand (without touching it).
*/
Card BotInfo::lookAtHand(size_t cardIndex) const
{
	if (this->cardsRevealed && cardIndex < this->hand.size())
	{
		return this->hand[cardIndex];
	}

	return Card::getNullCard();
}

