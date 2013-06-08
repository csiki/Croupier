#include "stdafx.h"
#include "Card.h"

Card Card::getNullCard()
{
	return Card(Card::Suit::NULLSUIT, Card::Rank::NULLRANK);
}

bool Card::isNullCard(Card& card)
{
	return false;
}

int Card::getVal() const
{
	return 100 * this->suit + this->rank;
}