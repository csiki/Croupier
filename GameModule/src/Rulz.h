#if !defined(_RULZ_H)
#define _RULZ_H

using namespace std;

// TODO gondolkodni, hogy kellenek e még szabályok, mert egy plusz szabály nem csak egy bejegyzés (pl. BotCommunicator...)

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
	long allowedBotCalcTime;
	int startingChips;
public:
	int getBigBlind(int index) const;
	int getSmallBlind(int index) const;
	int getBlindShiftDeadline(int index) const;
	int getNumOfBlinds() const;
	int getNumOfRebuysAllowed() const;
	int getRebuyDeadline() const;
	bool isTalkAllowed() const;
	bool isEmotionAllowed() const;
	bool isBotKnowledgeUseAllowed() const;
	long getAllowedBotCalcTime(int langID) const;
	int getStartingChips() const;
};

#endif  //_RULZ_H
