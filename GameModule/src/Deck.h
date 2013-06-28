#if !defined(_DECK_H)
#define _DECK_H

#include "Card.h"

using namespace std;

/**	Deck of cards.
*/
class Deck
{
private:
	vector<Card*> cards;
public:

	/** Fills deck.
	*/
	Deck()
	{
		for (int i = 1; i <= 4; i++)
		{
			for (int j = 2; j <= 14; j++)
			{
				this->cards.push_back(new Card(i, j));
			}
		}
	}

	int num() const;
	Card* pop();
	void push(Card* c);
	void shuffle();
};

#endif  //_DECK_H
