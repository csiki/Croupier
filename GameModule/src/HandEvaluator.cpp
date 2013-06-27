#include "stdafx.h"
#include "HandEvaluator.h"
#include "HandRank.h"

/** Determine the highest rank of given 5 cards.
*/
HandRank HandEvaluator::evalFiveCards(const Card** cards)
{
	// using a histogram, we check if we got FourOfAKind, FullHouses, ThreeOfAKind, TwoPair or Pair
	// building a histogram of the 5 cards rank
	int histogram[5] = {0, 0, 0, 0, 0};
	bool alreadyFound;
	
	for (int c = 0; c < 5; ++c)
	{
		/*
			i. element of the histogram is represents
			the number of cards with the same rank as
			the card at i. position in cards array,
			if it's the first occurence of the card
			in cards.
		*/

		// from 0 to c we check if there's already a card with the same rank
		alreadyFound = false;
		for (int i = 0; i < c; ++i)
		{
			if (cards[i]->rank == cards[c]->rank)
			{
				// if found one we increase that one
				++histogram[i];
				alreadyFound = true;
				break;
			}
		}

		if (!alreadyFound)
		{
			// if there's no card found with the same rank before
			++histogram[c];
		}
	}

	// sort decreasing the histogram
	std::sort(histogram, histogram + 5, std::greater<int>());

	// check if FourOfAKind, FullHouses, ThreeOfAKind, TwoPair, Pair
	if (histogram[0] == 4)
	{
		// like 4,1,0,0,0
		return HandRank::FourOfAKind;
	}
	else if (histogram[0] == 3 && histogram[1] == 2)
	{
		// like 3,2,0,0,0
		return HandRank::FullHouses;
	}
	else if (histogram[0] == 3) // && histogram[1] == 1)
	{
		// like 3,1,1,0,0
		return HandRank::ThreeOfAKind;
	}
	else if (histogram[0] == 2 && histogram[1] == 2)
	{
		// like 2,2,1,0,0
		return HandRank::TwoPair;
	}
	else if (histogram[0] == 2) // && histogram[1] == 1)
	{
		// like 2,1,1,1,0
		return HandRank::Pair;
	}

	// check for flush
	bool isFlush = true;
	for (int i = 1; i < 5; ++i)
	{
		// all cards must have equal rank
		if (cards[0]->rank != cards[i]->rank)
		{
			isFlush = false;
			break;
		}
	}

	// check for straight(flush)
	/*
		If the difference between the highest and the lowest
		cards rank is 4, than is's a straight (or it should
		be a pair or something, but we've already drop them out).
	*/
	// find highest and lowest card rank
	int highrank, lowrank;
	highrank = cards[0]->rank;
	lowrank = cards[0]->rank;
	for (int i = 1; i < 5; ++i)
	{
		if (highrank < cards[i]->rank)
		{
			highrank = cards[i]->rank;
		}
		
		if (cards[i]->rank < lowrank)
		{
			lowrank = cards[i]->rank;
		}
	}

	// if the difference is 4, it's a straight(flush)
	if ( (highrank - lowrank) == 4 )
	{
		if (isFlush)
		{
			return HandRank::StraightFlush;
		}

		return HandRank::Straight;
	}

	return HandRank::HighCard;
}

/** Compares two cards by their rank.
*/
int HandEvaluator::cardComparatorByRank(const Card* c1, const Card* c2)
{
	if (c2->rank < c1->rank)
	{
		return 1;
	}
	else if (c1->rank < c2->rank)
	{
		return -1;
	}

	return 0;
}

/** Determine the best combination of 5 cards from 7, and returns with its highest rank.
 *	@param cards 7 cards, 2 in bot's hand, 5 from table
 *	@param bestHand already allocated for the best combination of 5 cards of cards
 *	@return rank of bestHand
*/
HandRank HandEvaluator::evalHand(const Card** cards, const Card** bestHand)
{
	const Card* tmpHand[5];
	HandRank bestRank = HandRank::None;
	HandRank tmpRank;
	int tmp;

	// all combination of 5 cards from 7, choosing two cards of the 5 (c1, c2)
	for (int c1 = 0; c1 < 6; ++c1)
	{
		for (int c2 = c1+1; c2 < 7; ++c2)
		{
			// now the two cards c1, and c2 chosen out of the 7
			// tmpHand with 5 cards
			tmp = 0;
			for (int i = 0; i < 7; ++i)
			{
				if (i != c1 && i != c2)
				{
					tmpHand[tmp++] = cards[i];
				}
			}

			// rank tmpHand
			tmpRank = this->evalFiveCards(tmpHand);

			if (bestRank < tmpRank) // if higher rank found
			{
				bestRank = tmpRank;
				
				// fill bestHand with tmpHand
				for (int i = 0; i < 5; ++i)
				{
					bestHand[i] = tmpHand[i];
				}
			}
			else if (bestRank == tmpRank) // equally high rank found
			{
				// compare them to make sure which is better
				if (this->handComparator(bestRank, bestHand, tmpHand) == -1) // bestHand < tmpHand
				{
					// fill bestHand with tmpHand
					for (int i = 0; i < 5; ++i)
					{
						bestHand[i] = tmpHand[i];
					}
				}
			}
		}
	}

	return bestRank;
}

/** Compares 5-5 cards, with the same rank.
*/
int HandEvaluator::handComparator(HandRank rank, const Card** bestHand1, const Card** bestHand2)
{
	// sort both bestHands cards by rank
	// easier to compare when you know where is the highest/lowest cards
	std::sort(bestHand1, bestHand1+5, cardComparatorByRank);
	std::sort(bestHand2, bestHand2+5, cardComparatorByRank);

	// we compare the cards differently according to rank
	if (rank == HandRank::Straight || rank == HandRank::StraightFlush) // straight
	{
		// check the highest card
		if (bestHand2[4]->rank < bestHand1[4]->rank)
		{
			return 1;
		}
		else if (bestHand1[4]->rank < bestHand2[4]->rank)
		{
			return -1;
		}
	}
	else if (rank == HandRank::Flush) // flush
	{
		// iterate from the highest to the lowest
		// check at given position, which card is higher
		for (int i = 4; i >= 0; --i)
		{
			if (bestHand2[i]->rank < bestHand1[i]->rank)
			{
				return 1;
			}
			else if (bestHand1[i]->rank < bestHand2[i]->rank)
			{
				return -1;
			}
		}
	}
	else if (rank == HandRank::Pair) // pair
	{
		int pair1rank, pair2rank;
		pair1rank = pair2rank = 0;

		// find the the rank of the pair (the rank of the 2 cards making the pair) for bestHand1
		for (int i = 0; i < 4; ++i)
		{
			if (bestHand1[i]->rank == bestHand1[i+1]->rank)
			{
				pair1rank = bestHand1[i]->rank;
				break;
			}
		}

		// find the the rank of the pair for bestHand2
		for (int i = 0; i < 4; ++i)
		{
			if (bestHand2[i]->rank == bestHand2[i+1]->rank)
			{
				pair2rank = bestHand2[i]->rank;
				break;
			}
		}

		// highest hand is in which the pair rank is higher
		if (pair2rank < pair1rank)
		{
			return 1;
		}
		else if (pair1rank < pair2rank)
		{
			return -1;
		}

		// iterate from the highest to the lowest
		// check at given position, which card is higher
		for (int i = 4; i >= 0; --i)
		{
			if (bestHand2[i]->rank < bestHand1[i]->rank)
			{
				return 1;
			}
			else if (bestHand1[i]->rank < bestHand2[i]->rank)
			{
				return -1;
			}
		}
	}
	else if (rank == HandRank::TwoPair) // two pair
	{
		// pair12rank means for example the rank of the higher pair in bestHand1
		int pair11rank, pair12rank, pair21rank, pair22rank;
		pair11rank = pair12rank = pair21rank = pair22rank = 0;

		// kicker is the only card that neither of the two pairs contains
		int kicker1rank, kicker2rank;
		kicker1rank = kicker2rank = 0;

		// find pair11rank, pair12rank and kicker1
		for (int i = 0; i < 4; ++i)
		{
			// two cards are with the same rank
			if (bestHand1[i]->rank == bestHand1[i+1]->rank)
			{
				if (pair11rank == 0) // if we haven't found the first pair
				{
					pair11rank = bestHand1[i]->rank;
				}
				else // we've already found the first, it's the second pair
				{
					pair12rank = bestHand1[i]->rank;
				}

				// increment, to not recognize the second card of a pair, a kicker
				++i;
			}
			else // it's a kicker
			{
				kicker1rank = bestHand1[i]->rank;
			}
		}

		// find pair21rank, pair22rank and kicker2
		for (int i = 0; i < 4; ++i)
		{
			// two cards are with the same rank
			if (bestHand2[i]->rank == bestHand2[i+1]->rank)
			{
				if (pair21rank == 0) // if we haven't found the first pair
				{
					pair21rank = bestHand2[i]->rank;
				}
				else // we've already found the first, it's the second pair
				{
					pair22rank = bestHand2[i]->rank;
				}

				// increment, to not recognize the second card of a pair, a kicker
				++i;
			}
			else // it's a kicker
			{
				kicker2rank = bestHand2[i]->rank;
			}
		}

		// because of the sorting, pair11rank < pair12rank always etc.
		// at first we compare the higher pair ranks
		if (pair22rank < pair12rank)
		{
			return 1;
		}
		else if (pair12rank < pair22rank)
		{
			return -1;
		}

		// now we compare the lower pair ranks
		if (pair21rank < pair11rank)
		{
			return 1;
		}
		else if (pair11rank < pair21rank)
		{
			return -1;
		}

		// at the end we compare the kickers
		if (kicker2rank < kicker1rank)
		{
			return 1;
		}
		else if (kicker2rank < kicker1rank)
		{
			return -1;
		}
	}
	else if (rank == ThreeOfAKind) // drill
	{
		// three rank is the rank of the cards making the drill
		int three1rank, three2rank;
		three1rank = three2rank = 0;

		// find three1rank
		for (int i = 0; i < 4; ++i)
		{
			if (bestHand1[i]->rank == bestHand1[i+1]->rank)
			{
				// if two cards rank equals, it means they must be part of the drill
				three1rank = bestHand1[i]->rank;
				break;
			}
		}

		// find three2rank
		for (int i = 0; i < 4; ++i)
		{
			if (bestHand2[i]->rank == bestHand2[i+1]->rank)
			{
				// if two cards rank equals, it means they must be part of the drill
				three2rank = bestHand2[i]->rank;
				break;
			}
		}

		// compare three1rank and three2rank
		if (three1rank < three2rank)
		{
			return 1;
		}
		else if (three2rank < three1rank)
		{
			return -1;
		}

		// iterate from the highest to the lowest
		// check at given position, which card is higher
		for (int i = 4; i >= 0; --i)
		{
			if (bestHand2[i]->rank < bestHand1[i]->rank)
			{
				return 1;
			}
			else if (bestHand1[i]->rank < bestHand2[i]->rank)
			{
				return -1;
			}
		}
	}
	else if (rank == HandRank::FullHouses) // fullhouses
	{
		// pair rank is the rank of the cards making the pair of fullhouse
		// three rank is the rank of the cards making the drill of fullhouse
		int pair1rank, pair2rank, three1rank, three2rank;
		pair1rank = pair2rank = three1rank = three2rank = 0;

		// finding out pair and three ranks of besthand1
		if (bestHand1[0]->rank == bestHand1[2]->rank)
		{
			// like t t t p p
			three1rank = bestHand1[0]->rank;
			pair1rank = bestHand1[3]->rank;
		}
		else
		{
			// like p p t t t
			three1rank = bestHand1[2]->rank;
			pair1rank = bestHand1[0]->rank;
		}

		// finding out pair and three ranks of besthand2
		if (bestHand2[0]->rank == bestHand2[2]->rank)
		{
			// like t t t p p
			three2rank = bestHand2[0]->rank;
			pair2rank = bestHand2[3]->rank;
		}
		else
		{
			// like p p t t t
			three2rank = bestHand2[2]->rank;
			pair2rank = bestHand2[0]->rank;
		}

		// first we compare the rank of the drill
		if (three2rank < three1rank)
		{
			return 1;
		}
		else if (three1rank < three2rank)
		{
			return -1;
		}

		// now we compare the rank of the pair
		if (pair2rank < pair1rank)
		{
			return 1;
		}
		else if (pair1rank < pair2rank)
		{
			return -1;
		}
	}
	else if (rank == FourOfAKind) // poker
	{
		// poker rang is the rang of the cards making the poker
		int poker1rank, poker2rank;
		poker1rank = poker2rank = 0;
		// kicker is the remaining one card
		int kicker1rank, kicker2rank;
		kicker1rank = kicker2rank = 0;

		// find poker rank and kicker rank of bestHand1
		if (bestHand1[0]->rank == bestHand1[1]->rank)
		{
			// like p p p p k
			poker1rank = bestHand1[0]->rank;
			kicker1rank = bestHand1[4]->rank;
		}
		else
		{
			// like k p p p p
			poker1rank = bestHand1[1]->rank;
			kicker1rank = bestHand1[0]->rank;
		}

		// find poker rank and kicker rank of bestHand2
		if (bestHand2[0]->rank == bestHand2[1]->rank)
		{
			// like p p p p k
			poker2rank = bestHand2[0]->rank;
			kicker2rank = bestHand2[4]->rank;
		}
		else
		{
			// like k p p p p
			poker2rank = bestHand2[1]->rank;
			kicker2rank = bestHand2[0]->rank;
		}

		// at first we compare the rank of the poker
		if (poker2rank < poker1rank)
		{
			return 1;
		}
		else if (poker1rank < poker2rank)
		{
			return -1;
		}

		// now compare the kickers
		if (kicker2rank < kicker1rank)
		{
			return 1;
		}
		else if (kicker1rank < kicker2rank)
		{
			return -1;
		}
	}
	else if (rank == HandRank::HighCard) // high card
	{
		// iterate from the highest to the lowest
		// check at given position, which card is higher
		for (int i = 4; i >= 0; --i)
		{
			if (bestHand2[i]->rank < bestHand1[i]->rank)
			{
				return 1;
			}
			else if (bestHand1[i]->rank < bestHand2[i]->rank)
			{
				return -1;
			}
		}
	}

	// it's a tie
	return 0;
}

