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
Card* Deck::pop()
{
	if (cards.size() == 0)
	{
		throw "The deck is empty."; // TEST
	}

	Card* c = cards.back();
	cards.pop_back();
	return c;
}

/** Push a card to the deck.
*/
void Deck::push(Card* c)
{
	if (cards.size() == 52)
	{
		throw "The deck is full."; // TEST
	}

	cards.push_back(c);
}

/** Shuffles deck.
*/
void Deck::shuffle()
{
	if (cards.size() != 52)
	{
		throw "The deck is not complete.";
	}

	srand((unsigned int)time(0));

	std::random_shuffle(cards.begin(), cards.end()); // using inner Fisher–Yates shuffle algorithm
}