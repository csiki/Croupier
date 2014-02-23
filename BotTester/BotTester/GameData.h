#if !defined(_GAMEDATA_H)
#define _GAMEDATA_H

#include "BotData.h"
#include <vector>

/** Provide all the data needed for starting a game.
*/
class GameData
{
private:
	std::vector<BotData*> bots;

public:
	const int id;
	const std::string logFileName;
	const std::string rulzFileName;
	const std::string resultsFileName;

	GameData(int id, const char* logFileName, const char* rulzFileName, const char* resultsFileName)
		: id(id), logFileName(logFileName), rulzFileName(rulzFileName), resultsFileName(resultsFileName)
	{}

	~GameData()
	{
		for (size_t i = 0; i < bots.size(); ++i)
		{
			delete bots[i];
		}
	}

	void addBotData(BotData* bd);
	BotData* getBotData(int index);
	size_t getNumOfBots();
};

#endif
