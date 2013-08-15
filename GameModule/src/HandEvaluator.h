#if !defined(_HANDEVALUATOR_H)
#define _HANDEVALUATOR_H

#include "HandRank.h"
#include "Card.h"

/**	Evaluates and compares hands.
*/
class HandEvaluator
{
private:
	static HandRank evalFiveCards(const Card** cards); // done

public:
	static HandRank evalHand(const Card** cards, const Card** bestHand); // UNIT done
	static int handComparator(HandRank rank, const Card** bestHand1, const Card** bestHand2); // UNIT done
	static bool cardComparatorByRank(const Card* c1, const Card* c2); // UNIT done
};

#endif  //_HANDEVALUATOR_H
