#include "stdafx.h"
#include "GameOwner.h"

/** Construct all the components of a game.
*/
bool GameOwner::initialiseGame()
{
	// create independent components
	this->log = new Log();
	this->rulz = RulzXMLHandler::loadXML(this->rulzPath);
	
	if (this->rulz == nullptr)
	{
		this->gameState = 5; // fatal error
		this->errorMsg = "Cannot load Rulz instance from xml!";
		return false;
	}

	this->broadcastStation = new BroadcastStation;
	this->table = new Table(this->numOfBots);

	// load bots
	for (int i = 0; i < this->numOfBots; ++i)
	{

	}

	return true;
}

/** Trigger the croupier mainly, to start the game.
*/
int GameOwner::startGame()
{
	return 0;
}

/** Save the results of the game.
*/
void GameOwner::saveResults()
{
}

/** Returns state of the game.
*/
int GameOwner::getGameState() const
{
	return this->gameState;
}

/** Gets the message of the last error occured.
*/
string GameOwner::getErrorMsg() const
{
	return this->errorMsg;
}