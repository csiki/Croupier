#if !defined(_HANDEVALUATOR_H)
#define _HANDEVALUATOR_H

#include "HandRank.h"
#include "Card.h"

/**	Evaluates and compares hands.
*/
class HandEvaluator
{
private:
	HandRank evalFiveCards(const Card** cards);

public:
	HandRank evalHand(const Card** cards, const Card** bestHand); // UNIT
	int handComparator(HandRank rank, const Card** bestHand1, const Card** bestHand2); // UNIT
	static int cardComparatorByRank(const Card* c1, const Card* c2); // UNIT
};

#endif  //_HANDEVALUATOR_H
