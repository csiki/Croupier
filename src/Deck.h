#if !defined(_DECK_H)
#define _DECK_H

#include "Card.h"

class Deck
{
private:
	Card cards;
public:
	int num();
	const Card* const pop();
	void push(const Card* const c);
	void shuffle();
};

#endif  //_DECK_H
