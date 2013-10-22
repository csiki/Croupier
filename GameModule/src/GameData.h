#if !defined(_GAMEDATA_H)
#define _GAMEDATA_H

#include "BotData.h"

/** Provide all the data needed for starting a game.
*/
class GameData
{
private:
	std::vector<BotData*> bots;

public:
	const int id;
	const char* logFileName;
	const char* rulzFileName;
	const char* resultsFileName;

	GameData(int id, const char* logFileName, const char* rulzFileName, const char* resultsFileName)
		: id(id), logFileName(logFileName), rulzFileName(rulzFileName), resultsFileName(resultsFileName)
	{}

	void addBotData(BotData* bd);
	BotData* getBotData(int index);
	int getNumOfBots();
};

#endif