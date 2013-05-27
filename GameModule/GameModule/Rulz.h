#if !defined(_RULZ_H)
#define _RULZ_H

#include <vector>

using namespace std;

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
	bool knowledgeBaseAllowed;
	int maxNumOfRaises;
	long allowedBotCalcTime;
	int startingChips;
public:
	int getBigBlind(int index);
	int getSmallBlind(int index);
	int getBlindShiftDeadline(int index);
	int getNumOfBlinds();
	int getNumOfRebuysAllowed();
	int getRebuyDeadline();
	bool isTalkAllowed();
	bool isKnowledgeBaseAllowed();
	long getAllowedBotCalcTime();
	int getStartingChips();
};

#endif  //_RULZ_H
