#include "stdafx.h"
#include "GameDataXMLHandler.h"

/** From an xml file, loads a GameData instance.
*/
GameData* GameDataXMLHandler::loadXML(std::string xmlPath)
{
	// load file, check if file exists
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlPath.c_str());

	GameData* gameData = nullptr;
	if (result)
	{
		// collect data from xml
		pugi::xpath_node gameNode = doc.select_single_node("/game");

		const int id = gameNode.node().child("id").text().as_int();
		const char* logPath = gameNode.node().child("log").text().as_string();
		const char* rulzPath = gameNode.node().child("rules").text().as_string();
		const char* resultsPath = gameNode.node().child("results").text().as_string();

		gameData = new GameData(id, logPath, rulzPath, resultsPath);

		// load bots data
		pugi::xml_node botsNode = gameNode.node().child("bots");
		for (pugi::xml_node botNode = botsNode.child("bot"); botNode; botNode = botNode.next_sibling("bot"))
		{
			gameData->addBotData(BotDataXMLHandler::loadXML(botNode));
		}
	}

	return gameData;
}

/** Saves a GameData instance as an xml file.
*/
bool GameDataXMLHandler::saveXML(GameData* gameData, std::string xmlPath)
{
	if (gameData != nullptr)
	{
		pugi::xml_document doc;
		pugi::xml_node gameNode = doc.append_child("game");

		// save one instance propeties
		gameNode.append_child("id").text().set(gameData->id);
		gameNode.append_child("log").text().set(gameData->logFileName.c_str());
		gameNode.append_child("rules").text().set(gameData->rulzFileName.c_str());
		gameNode.append_child("results").text().set(gameData->resultsFileName.c_str());

		// save bot data
		pugi::xml_node botsNode = gameNode.append_child("bots");
		for (size_t i = 0; i < gameData->getNumOfBots(); ++i)
		{
			pugi::xml_node botNode = botsNode.append_child("bot");
			BotDataXMLHandler::saveXML(gameData->getBotData(i), botNode);
		}

		// save xml
		return doc.save_file(xmlPath.c_str());
	}

	return false;
}
