#include "stdafx.h"
#include "Card.h"

/** Returns a null card.
*/
Card Card::getNullCard()
{
	return Card(Card::Suit::NULLSUIT, Card::Rank::NULLRANK);
}

/** Returns if a given card is a null card.
*/
bool Card::isNullCard(Card& card)
{
	if (card.suit == Card::Suit::NULLSUIT || card.rank == Card::Rank::NULLRANK)
	{
		return true;
	}

	return false;
}

/** Returns unique value of a card.
 * @return 100*suit + rank
*/
int Card::getVal() const
{
	return 100 * this->suit + this->rank;
}