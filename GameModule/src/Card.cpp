#include "stdafx.h"
#include "Card.h"

Card Card::getNullCard()
{
	return Card(Card::Suit::NULLSUIT, Card::Rank::NULLRANK);
}

bool Card::isNullCard(Card& card)
{
	if (card.suit == Card::Suit::NULLSUIT || card.rank == Card::Rank::NULLRANK)
	{
		return true;
	}
	return false;
}

int Card::getVal() const
{
	return 100 * this->suit + this->rank;
}