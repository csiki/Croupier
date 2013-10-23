#include "stdafx.h"
#include "AliveBotLoader.h"
#include "AliveBot.h"

Bot* AliveBotLoader::loadBot(BotData* botData)
{
	return new AliveBot(botData->communicator, botData->id, botData->name, botData->lang);
}

Process* AliveBotLoader::initProcess()
{
	return nullptr;
}