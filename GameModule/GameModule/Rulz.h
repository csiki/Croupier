#if !defined(_RULZ_H)
#define _RULZ_H

#include <vector>

using namespace std;

class Rulz
{
private:
	vector<int> smallBlinds;
	vector<int> bigBlinds;
	int rebuyDeadline;
	vector<int> blindShiftDeadlines;
	int numOfRebuysAllowed;
	bool talkAllowed;
	bool knowledgeBaseAllowed;
	int maxNumOfRaises;
	long allowedBotCalcTime;
	int startingChips;
public:
	int getBigBlind(int index);
	int getBlindShiftDeadline(int index);
	int getNumOfRebuysAllowed();
	int getRebuyDeadline();
	int getSmallBlind(int index);
	bool isTalkAllowed();
	bool isKnowledgeBaseAllowed();
	long getAllowedBotCalcTime();
	int getStartingChips();
};

#endif  //_RULZ_H
