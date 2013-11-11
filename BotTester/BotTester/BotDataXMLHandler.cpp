#include "BotDataXMLHandler.h"
#include <list>

/** From an xml file, loads a BotData instance.
*/
BotData* BotDataXMLHandler::loadXML(pugi::xml_node& botNode)
{
	BotData* botData = nullptr;

	int id, playerID, credit, numOfKnowledgeTables;
	BotLanguage lang;
	int *knowledgeTables;
	std::string name, src;

	// load single instance data
	id = botNode.child("id").text().as_int();
	playerID = botNode.child("playerid").text().as_int();
	name = botNode.child("name").text().as_string();
	src = botNode.child("src").text().as_string();
	lang = static_cast<BotLanguage>( botNode.child("lang").text().as_int() );
	credit = botNode.child("credit").text().as_int();

	// load knowledge tables
	std::list<int> tempList;
	pugi::xml_node ktNode = botNode.child("knowledgetables");
	for (pugi::xml_node tableIDNode = ktNode.child("tableid"); tableIDNode; tableIDNode = tableIDNode.next_sibling("tableid"))
	{
		tempList.push_back(tableIDNode.text().as_int());
	}
	numOfKnowledgeTables = tempList.size();

	knowledgeTables = new int[numOfKnowledgeTables];
	int i = 0;
	for (std::list<int>::iterator it = tempList.begin(); it != tempList.end(); ++it)
	{
		knowledgeTables[i++] = *it;
	}

	botData = new BotData(id, playerID, name, src, credit, lang,
		numOfKnowledgeTables, knowledgeTables);

	return botData;
}

/** Saves a BotData instance as an xml file.
  * DEPRICATED !
*/
bool BotDataXMLHandler::saveXML(BotData* botData, pugi::xml_node& botNode)
{
	if (botData != nullptr)
	{
		// save one instance propeties
		botNode.append_child("id").text().set(botData->id);
		botNode.append_child("playerid").text().set(botData->playerID);
		botNode.append_child("name").text().set(botData->name.c_str());
		botNode.append_child("src").text().set(botData->src.c_str());
		botNode.append_child("lang").text().set(botData->lang);
		botNode.append_child("credit").text().set(botData->credit);

		// save knowledge tables
		pugi::xml_node ktNode = botNode.append_child("knowledgetables");
		for (size_t i = 0; i < botData->numOfKnowledgeTables; ++i)
		{
			ktNode.append_child("tableid").text().set(botData->knowledgeTables[i]);
		}

		return true;
	}

	return false;
}
