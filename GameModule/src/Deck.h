#if !defined(_DECK_H)
#define _DECK_H

#include <list>
#include "Card.h"
#include <memory>

using namespace std;

/**	Deck of cards.
*/
class Deck
{
private:
	list<auto_ptr<Card>> cards;
public:
	Deck();
	int num() const;
	Card* pop();
	void push(Card* c);
	void shuffle();
};

#endif  //_DECK_H
