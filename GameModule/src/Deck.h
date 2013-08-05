#if !defined(_DECK_H)
#define _DECK_H

#include "Card.h"

/**	Deck of cards.
*/
class Deck
{
private:
	vector<Card*> cards;

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
		for (vector<Card*>::iterator it = cards.begin(); it != cards.end(); ++it)
		{
			delete *it;
		}
	}

	int num() const;
	Card* pop();
	void push(Card* c);
	void shuffle();
};

#endif  //_DECK_H
