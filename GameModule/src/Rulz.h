#if !defined(_RULZ_H)
#define _RULZ_H

#include "BettingSystem.h"

// TODO attribútumok constansnak ?

/**	Contains all the rules of a game.
*/
class Rulz
{
private:
	int* smallBlinds;
	int* bigBlinds;
	int* blindShiftDeadlines;
	int numOfBlinds;
	int rebuyDeadline;
	int numOfRebuysAllowed;
	bool talkAllowed;
	bool emotionAllowed;
	bool knowledgeBaseAllowed;
	int maxNumOfRaises;
	int *allowedBotCalcTime;
	int startingChips;
	BettingSystem bs;
public:

	Rulz(BettingSystem bs, int numOfBlinds, int* smallBlinds, int* bigBlinds, int* blindShiftDeadlines,
		int rebuyDeadline, int numOfRebuysAllowed, int startingChips, int maxNumOfRaises,
		int* allowedBotCalcTime, bool talkAllowed, bool emotionAllowed, bool knowledgeBaseAllowed)
	{
		this->bs = bs;
		this->numOfBlinds = numOfBlinds;
		this->smallBlinds = smallBlinds;
		this->bigBlinds = bigBlinds;
		this->blindShiftDeadlines = blindShiftDeadlines;
		this->rebuyDeadline = rebuyDeadline;
		this->numOfRebuysAllowed = numOfRebuysAllowed;
		this->startingChips = startingChips;
		this->maxNumOfRaises = maxNumOfRaises;
		this->allowedBotCalcTime = allowedBotCalcTime;
		this->talkAllowed = talkAllowed;
		this->emotionAllowed = emotionAllowed;
		this->knowledgeBaseAllowed = knowledgeBaseAllowed;
	}

	int getBigBlind(int index) const;
	int getSmallBlind(int index) const;
	int getBlindShiftDeadline(int index) const;
	int getNumOfBlinds() const;
	int getNumOfRebuysAllowed() const;
	int getRebuyDeadline() const;
	bool isTalkAllowed() const;
	bool isEmotionAllowed() const;
	bool isBotKnowledgeUseAllowed() const;
	int getAllowedBotCalcTime(int langID) const;
	int getStartingChips() const;
	BettingSystem getBettingSystem() const;
	int getMaxNumOfRaises() const;
};

#endif  //_RULZ_H
