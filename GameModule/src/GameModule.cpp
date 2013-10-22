#include "stdafx.h"
#include "GameModule.h"
#include "Comment.h"
#include "BotData.h"
#include "BotDataXMLHandler.h"

/** Belepo pont he!
    @param argv members in order: gameid
*/
int main(int argc, char* argv[])
{
	const int gameid = atoi(argv[1]);

	// TODO load game data

	// init game owner
	GameOwner* go = new GameOwner(numOfBots, playersID, botsID, logPath, rulesPath, resultsPath);
	go->initialiseGame();

	// run game
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

