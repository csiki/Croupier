#include "stdafx.h"
#include "HandEvaluator.h"
#include "HandRank.h"

HandRank HandEvaluator::evalFiveCards(const Card** cards)
{
	return HandRank::None;
}

HandRank HandEvaluator::evalHand(const Card** cards)
{
	return HandRank::None;
}

int HandEvaluator::handComparator(HandRank rank, const Card** bestHand1, const Card** bestHand2)
{
	return 0;
}

