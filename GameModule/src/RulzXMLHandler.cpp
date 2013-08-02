#include "stdafx.h"
#include "RulzXMLHandler.h"

/** Loads a Rulz object from an xml file.
*/
Rulz* RulzXMLHandler::loadXML(string xmlPath)
{
	// load file, check if file exists
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlPath.c_str());

	Rulz* rulz = nullptr;
	if (result)
	{
		// collect data from xml
		pugi::xpath_node rulzNode = doc.select_single_node("/rulz");

		bool talkAllowed, emoAllowed, knowledgeUseAllowed;
		int startingChips, maxNumOfRaises, numOfRebuysAllowed, rebuyDeadline;
		BettingSystem bs;
		int *smallBlinds, *bigBlinds, *shiftDeadlines, *botCalcTimes;
		int numOfBlinds;

		// one instance data
		talkAllowed = rulzNode.node().child("talkallowed").text().as_bool();
		emoAllowed = rulzNode.node().child("emoallowed").text().as_bool();
		knowledgeUseAllowed = rulzNode.node().child("knowledgeuseallowed").text().as_bool();
		startingChips = rulzNode.node().child("startingchips").text().as_int();
		maxNumOfRaises = rulzNode.node().child("maxnumofraises").text().as_int();
		numOfRebuysAllowed = rulzNode.node().child("numofrebuysallowed").text().as_int();
		rebuyDeadline = rulzNode.node().child("rebuydeadline").text().as_int();
		bs = (BettingSystem) rulzNode.node().child("bettingsystem").text().as_int();

		// arrays of data
		pugi::xpath_node blindsNode = doc.select_single_node("/rulz/blinds");
		
		// small blinds
		list<int> tempList;
		pugi::xml_node smallBlindsNode = blindsNode.node().child("small");
		for (pugi::xml_node blindNode = smallBlindsNode.child("blind"); blindNode; blindNode = blindNode.next_sibling("blind"))
		{
			tempList.push_back(blindNode.text().as_int());
		}
		numOfBlinds = tempList.size();

		smallBlinds = new int[tempList.size()];
		int i = 0;
		for (list<int>::iterator it = tempList.begin(); it != tempList.end(); ++it)
		{
			smallBlinds[i++] = *it;
		}

		// big blinds
		tempList.clear();
		pugi::xml_node bigBlindsNode = blindsNode.node().child("big");
		for (pugi::xml_node blindNode = bigBlindsNode.child("blind"); blindNode; blindNode = blindNode.next_sibling("blind"))
		{
			tempList.push_back(blindNode.text().as_int());
		}

		bigBlinds = new int[tempList.size()];
		i = 0;
		for (list<int>::iterator it = tempList.begin(); it != tempList.end(); ++it)
		{
			bigBlinds[i++] = *it;
		}

		// shift deadlines
		tempList.clear();
		pugi::xml_node shiftDeadlinesNode = blindsNode.node().child("shiftdeadlines");
		for (pugi::xml_node dlNode = shiftDeadlinesNode.child("dl"); dlNode; dlNode = dlNode.next_sibling("dl"))
		{
			tempList.push_back(dlNode.text().as_int());
		}

		shiftDeadlines = new int[tempList.size()];
		i = 0;
		for (list<int>::iterator it = tempList.begin(); it != tempList.end(); ++it)
		{
			shiftDeadlines[i++] = *it;
		}

		// bot calculation times
		tempList.clear();
		pugi::xpath_node botCalcNode = doc.select_single_node("/rulz/botcalctime");
		for (pugi::xml_node timeNode = botCalcNode.node().child("time"); timeNode; timeNode = timeNode.next_sibling("time"))
		{
			tempList.push_back(timeNode.text().as_int()); // in millisec
		}

		botCalcTimes = new int[tempList.size()];
		i = 0;
		for (list<int>::iterator it = tempList.begin(); it != tempList.end(); ++it)
		{
			botCalcTimes[i++] = *it;
		}

		// create rulz
		rulz = new Rulz(bs, numOfBlinds, smallBlinds, bigBlinds, shiftDeadlines,
			rebuyDeadline, numOfRebuysAllowed, startingChips, maxNumOfRaises, botCalcTimes,
			talkAllowed, emoAllowed, knowledgeUseAllowed);
	}

	return rulz;
}

/** Saves a Rulz object as an xml file.
*/
bool RulzXMLHandler::saveXML(Rulz* rules, string xmlPath)
{
	if (rules != nullptr)
	{
		pugi::xml_document doc;
		pugi::xml_node rulzNode = doc.append_child("rulz");

		// save one instance propeties
		rulzNode.append_child("talkallowed").text().set(rules->isTalkAllowed());
		rulzNode.append_child("emoallowed").text().set(rules->isEmotionAllowed());
		rulzNode.append_child("knowledgeuseallowed").text().set(rules->isBotKnowledgeUseAllowed());
		rulzNode.append_child("startingchips").text().set(rules->getStartingChips());
		rulzNode.append_child("maxnumofraises").text().set(rules->getMaxNumOfRaises());
		rulzNode.append_child("numofrebuysallowed").text().set(rules->getNumOfRebuysAllowed());
		rulzNode.append_child("rebuydeadline").text().set(rules->getRebuyDeadline());
		rulzNode.append_child("bettingsystem").text().set(rules->getBettingSystem());

		// save blinds (small and big)
		pugi::xml_node blindsNode = rulzNode.append_child("blinds");
		pugi::xml_node smallNode = blindsNode.append_child("small");
		pugi::xml_node bigNode = blindsNode.append_child("big");
		for (int i = 0; i < rules->getNumOfBlinds(); ++i)
		{
			smallNode.append_child("blind").text().set( rules->getSmallBlind(i) );
			bigNode.append_child("blind").text().set( rules->getBigBlind(i) );
		}

		// save blinds shifts (one less than the number of blinds)
		pugi::xml_node shiftDLsNode = blindsNode.append_child("shiftdeadlines");
		for (int i = 0; i < rules->getNumOfBlinds() - 1; ++i)
		{
			shiftDLsNode.append_child("dl").text().set( rules->getBlindShiftDeadline(i) );
		}

		// save bot calculation times
		pugi::xml_node botCalcNode = rulzNode.append_child("botcalctime");
		for ( int l = BotLanguage::CPP; ; ++l )
		{
			pugi::xml_node timeNode = botCalcNode.append_child("time");
			timeNode.text().set(rules->getAllowedBotCalcTime(l));
			timeNode.append_attribute("lang").set_value(l);

			if (l == BotLanguage::PHP)
			{
				break;
			}
		}

		// save xml
		return doc.save_file(xmlPath.c_str());
	}

	return false;
}

