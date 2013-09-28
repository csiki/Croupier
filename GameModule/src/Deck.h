#if !defined(_DECK_H)
#define _DECK_H

#include "Card.h"
#include <time.h>

/**	Deck of cards.
*/
class Deck
{
private:
	std::vector<Card*> cards;

public:
	/** Constructor of Deck.
	 *  Fills deck with cards.
	*/
	Deck()
	{
		for (int i = 1; i <= 4; ++i)
		{
			for (int j = 2; j <= 14; ++j)
			{
				this->cards.push_back(new Card(i, j));
			}
		}
	}

	/** Destructor of Deck.
	 *  Deletescards from deck.
	*/
	virtual ~Deck()
	{
		for (std::vector<Card*>::iterator it = cards.begin(); it != cards.end(); ++it)
		{
			delete *it;
		}
	}

	int num() const;
	Card* pop(); // UNIT done
	void push(Card* c); // UNIT done
	void shuffle(); // UNIT done
};

#endif  //_DECK_H
