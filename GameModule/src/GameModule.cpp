#include "stdafx.h"
#include "GameModule.h"
#include "Comment.h"
#include "BotData.h"
#include "BotDataXMLHandler.h"

int main(int argc, char* argv[])
{
	BotData* botData = BotDataXMLHandler::loadXML("botDataExample.xml");

	cout << botData->files[0] << endl;
	
	if (BotDataXMLHandler::saveXML(botData, "botDataEx2.xml"))
	{
		cout << botData->knowledgeTables[0] << endl;
		cout << botData->files[0] << endl;
	}

	return 0;
}

