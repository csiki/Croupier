#if !defined(_HANDEVALUATOR_H)
#define _HANDEVALUATOR_H

#include "HandRank.h"

class HandEvaluator
{
private:
	HandRank evalFiveCards(const Card** const cards);
public:
	HandRank evalHand(const Card** const cards);
	int handComparator(HandRank rank, const Card** const bestHand1, const Card** const bestHand2);
};

#endif  //_HANDEVALUATOR_H
