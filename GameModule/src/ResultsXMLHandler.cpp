#include "stdafx.h"
#include "ResultsXMLHandler.h"

Results* ResultsXMLHandler::loadXML(std::string xmlPath)
{
	// load file, check if file exists
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlPath.c_str());

	Results* results = nullptr;
	if (result)
	{
		// file loaded successfully
		pugi::xpath_node resultsNode = doc.select_single_node("/results");
		
		// load ids, credits, kicksatround
		std::vector<int> idTemp;
		std::vector<int> creditTemp;
		std::vector<int> kickAtRoundTemp;
		for (pugi::xml_node playerNode = resultsNode.node().child("player"); playerNode; playerNode = playerNode.next_sibling("player"))
		{
			idTemp.push_back(playerNode.child("id").text().as_int());
			creditTemp.push_back(playerNode.child("credit").text().as_int());
			kickAtRoundTemp.push_back(playerNode.child("kickatround").text().as_int());
		}

		// create Results instance and add player results
		results = new Results(idTemp.size());
		for (unsigned int i = 0; i < idTemp.size(); ++i)
		{
			results->addResult(idTemp.at(i), creditTemp.at(i), kickAtRoundTemp.at(i));
		}
	}

	return results;
}

bool ResultsXMLHandler::saveXML(Results* results, std::string xmlPath)
{
	if (results != nullptr)
	{
		pugi::xml_document doc;
		pugi::xml_node resultsNode = doc.append_child("results");

		// save player results
		for (int i = 0; i < results->getNumOfPlayers(); ++i)
		{
			pugi::xml_node playerNode = resultsNode.append_child("player");
			playerNode.append_child("id").text().set( results->getID(i) );
			playerNode.append_child("credit").text().set( results->getCredit(i) );
			playerNode.append_child("kickatround").text().set( results->getKickAtRound(i) );
		}

		// save xml
		return doc.save_file(xmlPath.c_str());
	}

	return false;
}