#include "stdafx.h"
#include "Card.h"

// init shared nullcard
Card Card::nullCard = Card();

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
	return card.suit == Card::Suit::NULLSUIT || card.rank == Card::Rank::NULLRANK;
}

/** Returns unique value of a card.
 * @return 100*suit + rank
*/
int Card::getVal() const
{
	return 100 * this->suit + this->rank;
}


std::string Card::toString() const
{
	std::string res;

	// rank
	switch (this->rank)
	{
	case Rank::ACE:
		res = "ACE of ";
		break;
	case Rank::DEUCE:
		res = "DEUCE of ";
		break;
	case Rank::EIGHT:
		res = "EIGHT of ";
		break;
	case Rank::FIVE:
		res = "FIVE of ";
		break;
	case Rank::FOUR:
		res = "FOUR of ";
		break;
	case Rank::JACK:
		res = "JACK of ";
		break;
	case Rank::KING:
		res = "KING of ";
		break;
	case Rank::NINE:
		res = "NINE of ";
		break;
	case Rank::QUEEN:
		res = "QUEEN of ";
		break;
	case Rank::SEVEN:
		res = "SEVEN of ";
		break;
	case Rank::SIX:
		res = "SIX of ";
		break;
	case Rank::TEN:
		res = "TEN of ";
		break;
	case Rank::THREE:
		res = "THREE of ";
		break;
	default:
		res = "NULL of ";
		break;
	}

	// suit
	switch (this->suit)
	{
	case Suit::CLUBS:
		res += "CLUBS";
		break;
	case Suit::DIAMONDS:
		res += "DIAMONDS";
		break;
	case Suit::HEARTS:
		res += "HEARTS";
		break;
	case Suit::SPADES:
		res += "SPADES";
		break;
	default:
		res += "NULL";
		break;
	}

	return res;
}

Card Card::getCardByVal(int val)
{
	Rank r = static_cast<Rank>(val % 100);
	Suit s = static_cast<Suit>((val - r) / 100);

	return Card(s, r);
}
