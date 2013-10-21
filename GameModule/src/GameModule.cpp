#include "stdafx.h"
#include "GameModule.h"
#include "Comment.h"
#include "BotData.h"
#include "BotDataXMLHandler.h"

/** Belepo pont he!
  @param argv elemei sorban: logPath, rulesPath, resultsPath, numOfBots, bot1XML, bot2XML...
*/
int main(int argc, char* argv[])
{
	const char* logPath = argv[1];
	const char* rulesPath = argv[2];
	const char* resultsPath = argv[3];
	int numOfBots = atoi(argv[4]);
	int* botsID;

	int* botsID = new int[3];
	botsID[0] = 1;
	botsID[1] = 2;
	botsID[2] = 3;
	GameOwner* go = new GameOwner(3, botsID, "testlog.xml", "testrules.xml", "testresults.xml");

	go->initialiseGame();

	if (go->getGameState() == 1) // initialised successfully
	{
		go->startGame();

		if (go->getGameState() == 3) // run successfully
		{
			go->saveResults();
		}
	}

	return go->getGameState();

}

