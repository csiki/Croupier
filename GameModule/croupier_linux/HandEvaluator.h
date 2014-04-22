#if !defined(_HANDEVALUATOR_H)
#define _HANDEVALUATOR_H

#include "HandRank.h"
#include "Card.h"

class HandEvaluator
{
  typedef std::pair<size_t, Card::Rank> CountRankPair;
  typedef std::vector<CountRankPair> Histogram;

  /*
   * Create a (sorted) histogram from the specified 5 card hand
   */
  static Histogram createHistogram(const std::vector<Card>& cards);

  /*
   * Calculate the value of the histogram
   * The histogram should be sorted based on the count and than the card rank, both decreasing
   */
  static unsigned getValueFromHistogram(const Histogram& histogram);

public:

	/*
	 * Evaluate the value of the hand to a 32bit unsigned integer
	 * Higher value means better hand and two hand have the same value if they are tied at the show down
	 */
	static unsigned evalHandValue(const std::vector<Card>& cards);

	/*
	 * Evaluate the rank of the hand
	 */
	static HandRank evalHandRank(const std::vector<Card>& cards);

	/*
   * Calculate the rank and the value of a hand (7 cards)
   */
  static std::pair<HandRank, unsigned> evalHand(const std::vector<Card>& cards);

  /*
   * Calculate the rank and the value of a 5 card hand
   */
  static std::pair<HandRank, unsigned> eval5CardHand(const std::vector<Card>& cards);
};

#endif  //_HANDEVALUATOR_H
