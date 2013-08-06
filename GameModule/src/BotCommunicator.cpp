#include "stdafx.h"
#include "BotCommunicator.h"

/**	Returns one of AI's card in hand.
*/
Card BotCommunicator::getHand(int cardIndex) const
{
	return Card::getNullCard();
}

/** Sets the AI's emotion.
*/
void BotCommunicator::setEmotion(Emotion emotion)
{
	this->emotion = emotion;
}