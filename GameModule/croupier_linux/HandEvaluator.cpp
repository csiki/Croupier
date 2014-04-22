#include "stdafx.h"
#include "HandEvaluator.h"
#include "HandRank.h"

unsigned HandEvaluator::evalHandValue(const std::vector<Card>& cards)
{
  return HandEvaluator::evalHand(cards).second;
}

HandRank HandEvaluator::evalHandRank(const std::vector<Card>& cards)
{
  return HandEvaluator::evalHand(cards).first;
}

std::pair<HandRank, unsigned> HandEvaluator::eval5CardHand(const std::vector<Card>& cards)
{
  Histogram hist = HandEvaluator::createHistogram(cards);

  HandRank rank = HandRank::HighCard;
  unsigned value = 0;

  if(hist.size() >= 1 && hist[0].first == 4)
  {
    rank = HandRank::FourOfAKind;
  }
  else if(hist.size() >= 2 && hist[0].first == 3 && hist[1].first == 2)
  {
    rank = HandRank::FullHouses;
  }
  else if(hist.size() >= 1 && hist[0].first == 3)
  {
    rank = HandRank::ThreeOfAKind;
  }
  else if(hist.size() >= 2 && hist[0].first == 2 && hist[1].first == 2)
  {
    rank = HandRank::TwoPair;
  }
  else if(hist.size() >= 1 && hist[0].first == 2)
  {
    rank = HandRank::Pair;
  }

  bool isFlush = true, isStraight = true;
  for(const Card& c : cards)
  {
    if(c.suit == Card::NULLSUIT)
    {
      isFlush = false;
      isStraight = false;
    }
    else if(c.suit != cards.front().suit)
    {
      isFlush = false;
    }
  }

  if(isStraight)
  {
    isStraight = false;
    if(hist.size() == 5)
    {
      if(hist[0].second - hist[4].second == 4)
      {
        isStraight = true;
      }
      else if(hist[0].second == Card::ACE && hist[1].second == Card::FIVE) //A->5 straight
      {
        isStraight = true;
        hist.push_back(hist[0]);
        hist.erase(hist.begin());
      }
    }
  }

  if(isFlush && isStraight)
  {
    if(hist[0].second == Card::ACE)
    {
      rank = HandRank::RoyalFlush;
    }
    else
    {
      rank = HandRank::StraightFlush;
    }
  }
  else if(isFlush)
  {
    rank = HandRank::Flush;
  }
  else if(isStraight)
  {
    rank = HandRank::Straight;
  }

  value = HandEvaluator::getValueFromHistogram(hist);
  value += rank << 20;

  return std::make_pair(rank, value);
}

std::pair<HandRank, unsigned> HandEvaluator::evalHand(const std::vector<Card>& cards)
{
  std::vector<Card> tmpHand;
  std::pair<HandRank, unsigned> bestValue(HandRank::None, 0), tmpValue;

  for(size_t s1 = 0; s1 < 7; ++s1)
  {
    for(size_t s2 = s1 + 1; s2 < 7; ++s2)
    {
      tmpHand.clear();
      for(size_t i = 0; i < 7; ++i)
      {
        if(i != s1 && i != s2)
        {
          tmpHand.push_back(cards[i]);
        }
      }
      tmpValue = HandEvaluator::eval5CardHand(tmpHand);
      bestValue = std::max(bestValue, tmpValue);
    }
  }
  return bestValue;
}

unsigned HandEvaluator::getValueFromHistogram(const Histogram& histogram)
{
  unsigned value = 0;
  for(size_t i = 0; i < histogram.size(); ++i)
  {
    value += histogram[i].second << (4 * (4 - i));
  }
  return value;
}

HandEvaluator::Histogram HandEvaluator::createHistogram(const std::vector<Card>& cards)
{
  Histogram hist;
  for(const Card& c : cards)
  {
    if(c.rank != Card::NULLRANK)
    {
      bool found = false;
      for(auto& it : hist)
      {
        if(it.second == c.rank)
        {
          ++it.first;
          found = true;
          break;
        }
      }
      if(!found)
      {
        hist.push_back(std::make_pair(1, c.rank));
      }
    }
  }
  std::sort(hist.begin(), hist.end(), std::greater<CountRankPair>());
  return hist;
}
