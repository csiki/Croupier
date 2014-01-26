#if !defined(_CARD_H)
#define _CARD_H

/**	Contains the suit and rank of a french card.
*/
class Card
{
public:

  enum Suit
  {
	NULLSUIT = 0,
    CLUBS = 1,
    DIAMONDS = 2,
    HEARTS = 3,
    SPADES = 4
  };

  enum Rank
  {
	NULLRANK = 0,
    DEUCE = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    TEN = 10,
    JACK = 11,
    QUEEN = 12,
    KING = 13,
    ACE = 14
  };

	const Suit suit;
	const Rank rank;

	Card() : suit(Suit::NULLSUIT), rank(Rank::NULLRANK) {}
	Card(int s, int r) : suit((Card::Suit)s), rank((Card::Rank)r) {}
	int getVal() const; // suit*100 + rank

	bool operator<(const Card& card) const { return this->rank < card.rank; }
	bool operator==(const Card& card) const { return this->rank == card.rank; }
	bool operator>(const Card& card) const { return this->rank > card.rank; }

	std::string toString() const;

	static Card getNullCard();
	static Card* getSharedNullCard();
	static bool isNullCard(Card& card);
	static Card getCardByVal(int val);

    static Card nullCard;
};

#endif  //_CARD_H
