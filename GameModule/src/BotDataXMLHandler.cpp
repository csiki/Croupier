#include "stdafx.h"
#include "BotDataXMLHandler.h"

/** From an xml file, loads a BotData instance.
*/
BotData* BotDataXMLHandler::loadXML(string xmlPath)
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
		string *files;

		// load single instance data
		id = botNode.node().child("id").text().as_int();
		lang = static_cast<BotLanguage>( botNode.node().child("lang").text().as_int() );
		credit = botNode.node().child("credit").text().as_int();
		
		// load knowledge tables
		list<int> tempList;
		pugi::xml_node ktNode = botNode.node().child("knowledgetables");
		for (pugi::xml_node tableIDNode = ktNode.child("tableid"); tableIDNode; tableIDNode = tableIDNode.next_sibling("tableid"))
		{
			tempList.push_back(tableIDNode.text().as_int());
		}
		numOfKnowledgeTables = tempList.size();

		knowledgeTables = new int[numOfKnowledgeTables];
		int i = 0;
		for (list<int>::iterator it = tempList.begin(); it != tempList.end(); ++it)
		{
			knowledgeTables[i++] = *it;
		}

		// load file names
		list<string> tempListStr;
		pugi::xml_node filesNode = botNode.node().child("files");

		for (pugi::xml_node fileNode = filesNode.child("file"); fileNode; fileNode = fileNode.next_sibling("file"))
		{
			tempListStr.push_back(fileNode.text().as_string());
		}
		numOfFiles = tempListStr.size();

		files = new string[numOfFiles];
		i = 0;
		for (list<string>::iterator it = tempListStr.begin(); it != tempListStr.end(); ++it)
		{
			files[i++] = *it;
		}

		botData = new BotData(id, credit, lang,
			numOfKnowledgeTables, knowledgeTables,
			numOfFiles, files);
	}

	return botData;
}

/** Saves a BotData instance as an xml file.
*/
bool BotDataXMLHandler::saveXML(BotData* botData, string xmlPath)
{
	if (botData != nullptr)
	{
		pugi::xml_document doc;
		pugi::xml_node botNode = doc.append_child("bot");

		// save one instance propeties
		botNode.append_child("id").text().set(botData->id);
		botNode.append_child("lang").text().set(botData->lang);

		// save knowledge tables
		pugi::xml_node ktNode = botNode.append_child("knowledgetables");
		for (int i = 0; i < botData->numOfKnowledgeTables; ++i)
		{
			ktNode.append_child("tableid").text().set(botData->knowledgeTables[i]);
		}

		// save file names
		pugi::xml_node filesNode = botNode.append_child("files");
		for (int i = 0; i < botData->numOfFiles; ++i)
		{
			filesNode.append_child("file").text().set(botData->files[i].c_str());
		}

		// save xml
		return doc.save_file(xmlPath.c_str());
	}

	return false;
}