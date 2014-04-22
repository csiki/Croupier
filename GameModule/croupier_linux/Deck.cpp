#include "stdafx.h"
#include "Deck.h"

/** Returns number of cards in deck.
*/
int Deck::num() const
{
	return cards.size();
}

/** Pop a card.
*/
Card Deck::pop()
{
	Card c = cards.back();
	cards.pop_back();
	return c;
}

/** Push a card to the deck.
*/
void Deck::push(const Card& c)
{
	cards.push_back(c);
}

/** Shuffles deck.
*/
void Deck::shuffle()
{
	std::random_shuffle(cards.begin(), cards.end()); // using inner Fisher-Yates shuffle algorithm
}
