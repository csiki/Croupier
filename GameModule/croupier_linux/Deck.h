#if !defined(_DECK_H)
#define _DECK_H

#include "Card.h"
#include <time.h>

/**	Deck of cards.
*/
class Deck
{
private:
	std::vector<Card> cards;

public:
	/*
	 *  Fills deck with cards.
	 */
	Deck()
	{
		for (size_t i = 1; i <= 4; ++i)
		{
			for (size_t j = 2; j <= 14; ++j)
			{
				this->cards.push_back(Card(i, j));
			}
		}
	}

	virtual ~Deck() {}

	int num() const;
	Card pop(); // UNIT done
	void push(const Card& c); // UNIT done
	void shuffle(); // UNIT done
};

#endif  //_DECK_H
