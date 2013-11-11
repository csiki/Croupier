#include "GameData.h"

/** Add a new bot data.
*/
void GameData::addBotData(BotData* bd)
{
	this->bots.push_back(bd);
}

/** Get an added bot data by index.
*/
BotData* GameData::getBotData(int index)
{
	return this->bots.at(index);
}

/** Get the number of bots added.
*/
size_t GameData::getNumOfBots()
{
	return this->bots.size();
}
