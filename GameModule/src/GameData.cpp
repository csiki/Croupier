#include "stdafx.h"
#include "GameData.h"

void GameData::addBotData(BotData* bd)
{
	this->bots.push_back(bd);
}

BotData* GameData::getBotData(int index)
{
	return this->bots.at(index);
}

int GameData::getNumOfBots()
{
	return this->bots.size();
}