#if !defined(_DECK_H)
#define _DECK_H

#include <list>
#include "Card.h"

using namespace std;

/**	Deck of cards.
*/
class Deck
{
private:
	list<Card*> cards;
public:
	int num() const;
	Card* pop();
	void push(Card* c);
	void shuffle();
};

#endif  //_DECK_H
