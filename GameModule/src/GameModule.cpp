#include "stdafx.h"
#include "GameModule.h"
#include "Comment.h"
#include "BotData.h"
#include "BotDataXMLHandler.h"

int main(int argc, char* argv[])
{
	//GameOwner gameowner(2, 1, 2, "testlog.xml,)
	
	int* playersID = new int[2];
	playersID[0] = 1;
	playersID[1] = 2;
	GameOwner* go = new GameOwner(2, playersID, "testlog.xml", "testrules.xml", "testresults.xml");

	go->test();

	return 0;
}

