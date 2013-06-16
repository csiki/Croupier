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
	Deck();
	int num() const;
	Card* pop();
	void push(Card* c);
	void shuffle();
};

#endif  //_DECK_H
