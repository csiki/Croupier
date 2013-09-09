#include "stdafx.h"
#include "GameModule.h"
#include "Comment.h"
#include "BotData.h"
#include "BotDataXMLHandler.h"

int main(int argc, char* argv[])
{
	//GameOwner gameowner(2, 1, 2, "testlog.xml,)
	
	int* playersID = new int[3];
	playersID[0] = 1;
	playersID[1] = 2;
	playersID[2] = 3;
	GameOwner* go = new GameOwner(3, playersID, "testlog.xml", "testrules.xml", "testresults.xml");

	go->initialiseGame();

	try {
		go->startGame();
	}
	catch (const char* e)
	{
		cout << e << endl;
	}

	go->saveResults();

	return 0;
}

