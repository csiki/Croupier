#include "stdafx.h"
#include "Deck.h"
#include <memory>
#include <algorithm>

Deck::Deck()
{
	for (int i = 1; i < 4; i++)
	{
		for (int j = 2; j < 14; j++)
		{
			cards.push_front((auto_ptr<Card>)(new Card(i, j)));
		}
	}
}

int Deck::num() const
{
	return cards.size();
}

Card* Deck::pop()
{
	if (cards.size() == 0)
	{
		throw "The deck is empty.";
	}
	Card* c = cards.front().get();
	cards.pop_front();
	return c;
}

void Deck::push(Card* c)
{
	if (cards.size() == 52)
	{
		throw "The deck is full.";
	}
	cards.push_front((auto_ptr<Card>)c);
}

void Deck::shuffle()
{
	if (cards.size() != 52)
	{
		throw "The deck is not complete.";
	}
	//TODO: ez csúnyán elhasal. valszeg a auto_ptr<Card> miatt.
	//std::random_shuffle(cards.begin(), cards.end()); //using inner Fisher–Yates shuffle algorithm
}

