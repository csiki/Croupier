#include "stdafx.h"
#include "BotDataXMLHandler.h"

/** From an xml file, loads a BotData instance.
*/
BotData* BotDataXMLHandler::loadXML(std::string xmlPath)
{
	// load file, check if file exists
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlPath.c_str());

	BotData* botData = nullptr;
	if (result)
	{
		// collect data from xml
		pugi::xpath_node botNode = doc.select_single_node("/bot");

		int id, credit, numOfKnowledgeTables, numOfFiles;
		BotLanguage lang;
		int *knowledgeTables;
		std::string file;
		std::string name;

		// load single instance data
		id = botNode.node().child("id").text().as_int();
		name = botNode.node().child("name").text().as_string();
		lang = static_cast<BotLanguage>( botNode.node().child("lang").text().as_int() );
		credit = botNode.node().child("credit").text().as_int();
		file = botNode.node().child("file").text().as_string();
		
		// load knowledge tables
		std::list<int> tempList;
		pugi::xml_node ktNode = botNode.node().child("knowledgetables");
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

		botData = new BotData(id, name, credit, lang,
			numOfKnowledgeTables, knowledgeTables, file);
	}

	return botData;
}

/** Saves a BotData instance as an xml file.
*/
bool BotDataXMLHandler::saveXML(BotData* botData, std::string xmlPath)
{
	if (botData != nullptr)
	{
		pugi::xml_document doc;
		pugi::xml_node botNode = doc.append_child("bot");

		// save one instance propeties
		botNode.append_child("id").text().set(botData->id);
		botNode.append_child("name").text().set(botData->name.c_str());
		botNode.append_child("lang").text().set(botData->lang);
		botNode.append_child("credit").text().set(botData->credit);
		botNode.append_child("file").text().set(botData->file.c_str());

		// save knowledge tables
		pugi::xml_node ktNode = botNode.append_child("knowledgetables");
		for (int i = 0; i < botData->numOfKnowledgeTables; ++i)
		{
			ktNode.append_child("tableid").text().set(botData->knowledgeTables[i]);
		}

		// save xml
		return doc.save_file(xmlPath.c_str());
	}

	return false;
}