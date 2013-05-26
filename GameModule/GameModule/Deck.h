#if !defined(_DECK_H)
#define _DECK_H

#include "Card.h"

class Deck
{
private:
	Card cards;
public:
	int num();
	Card* pop();
	void push(Card* c);
	void shuffle();
};

#endif  //_DECK_H
