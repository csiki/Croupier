#if !defined(_RULZ_H)
#define _RULZ_H

#include "BettingSystem.h"
#include "BotLanguage.h"

/**	Contains all the rules of a game.
*/
class Rulz
{
private:
	int* smallBlinds;
	int* bigBlinds;
	int* blindShiftDeadlines;
	size_t numOfBlinds;
	int rebuyDeadline;
	size_t numOfRebuysAllowed;
	bool talkAllowed;
	bool emotionAllowed;
	bool knowledgeBaseAllowed;
	std::map<BotLanguage, int> allowedBotCalcTime;
	int startingChips;
	BettingSystem bs;

public:
	Rulz(BettingSystem bs, int numOfBlinds, int* smallBlinds, int* bigBlinds, int* blindShiftDeadlines,
		int rebuyDeadline, int numOfRebuysAllowed, int startingChips,
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
		this->talkAllowed = talkAllowed;
		this->emotionAllowed = emotionAllowed;
		this->knowledgeBaseAllowed = knowledgeBaseAllowed;

		// fill allowedBotCalcTime
		for (size_t lang = BotLanguage::CPP; ; ++lang)
		{
			this->allowedBotCalcTime.insert(std::pair<BotLanguage, int>( static_cast<BotLanguage>(lang), allowedBotCalcTime[lang] ));
			if (lang == BotLanguage::ALIVE)
			{
				break;
			}
		}
	}

	virtual ~Rulz()
	{
		delete [] this->smallBlinds;
		delete [] this->bigBlinds;
		delete [] this->blindShiftDeadlines;
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
	int getAllowedBotCalcTime(BotLanguage lang) const;
	int getStartingChips() const;
	BettingSystem getBettingSystem() const;
};

#endif  //_RULZ_H
