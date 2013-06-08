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
	HandRank evalHand(const Card** cards);
	int handComparator(HandRank rank, const Card** bestHand1, const Card** bestHand2);
};

#endif  //_HANDEVALUATOR_H
