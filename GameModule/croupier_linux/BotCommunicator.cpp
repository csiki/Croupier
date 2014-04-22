#include "stdafx.h"
#include "BotCommunicator.h"

/**	Returns one of AI's card in hand.
*/
Card BotCommunicator::getHand(size_t cardIndex) const
{
	if (cardIndex < this->hand.size())
	{
		return this->hand[cardIndex];
	}
	return Card::getNullCard();
}

/** Sets the AI's emotion.
*/
void BotCommunicator::setEmotion(Emotion emotion)
{
	this->emotion = emotion;
}
