#if !defined(_HANDRANK_H)
#define _HANDRANK_H


enum HandRank
{
	None = 0,
	HighCard = 1,
	Pair = 2,
	TwoPair = 3,
	ThreeOfAKind = 4,
	Straight = 5,
	Flush = 6,
	FullHouses = 7,
	FourOfAKind = 8,
	StraightFlush = 9,
	RoyalFlush = 10
};

#endif  //_HANDRANK_H
