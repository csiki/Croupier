#include "stdafx.h"
#include "Deck.h"

/** Fills deck.
*/
Deck::Deck()
{
	for (int i = 1; i <= 4; i++)
	{
		for (int j = 2; j <= 14; j++)
		{
			this->cards.push_back(new Card(i, j));
		}
	}
}

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
		throw "The deck is empty.";
	}

	Card* c = cards.front();
	cards.pop_back();
	return c;
}

/** Push a card to the deck.
*/
void Deck::push(Card* c)
{
	if (cards.size() == 52)
	{
		throw "The deck is full.";
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

	std::random_shuffle(cards.begin(), cards.end()); // using inner Fisher–Yates shuffle algorithm
}

