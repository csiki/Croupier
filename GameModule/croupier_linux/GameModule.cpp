#include "stdafx.h"
#include "GameModule.h"
#include "Comment.h"
#include "BotData.h"
#include "BotDataXMLHandler.h"

/** Belepo pont he!
    @param argv members in order: gameDataXMLFileName
*/
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        // no input
<<<<<<< HEAD
        return 42;
=======
        return 6;
>>>>>>> dcd4efd73d2ca3b0cccb86f1f4ea3d54813031e5
    }

	// generate path of game data xml file
	std::string gameDataXMLFileName = _GAME_DATA_RELATIVE_PATH_;
	gameDataXMLFileName += argv[1];

	// init game owner
	GameOwner* go = new GameOwner(gameDataXMLFileName.c_str());
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

