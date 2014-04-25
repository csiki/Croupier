#include "stdafx.h"
#include "LogXMLHandler.h"

/** Loads a Log object from an xml file.
*/
std::vector<Event> LogXMLHandler::loadXML(std::string xmlPath)
{
  std::vector<Event> log;

	// load file, check if file exists
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlPath.c_str());

	if (result) // file loaded successfully
	{
		pugi::xpath_node logNode = doc.select_single_node("/log");

		// load events
		for (pugi::xml_node eventNode = logNode.node().child("event"); eventNode; eventNode = eventNode.next_sibling("event"))
		{
			int loggerID = eventNode.child("logger").text().as_int();
			const char* msg = eventNode.child("msg").text().as_string();
			Severity severity = static_cast<Severity>(eventNode.child("severity").text().as_int());

			log.push_back(Event(severity, loggerID, msg));
		}
	}

	return log;
}

/** Saves a Log object as an xml file.
*/
bool LogXMLHandler::saveXML(const std::vector<Event>& log,  std::string xmlPath)
{
  pugi::xml_document doc;
  pugi::xml_node logNode = doc.append_child("log");

  for (const auto& it : log)
  {
    // create event node
    pugi::xml_node eventNode = logNode.append_child("event");

    // add nodes to event
    eventNode.append_child("logger").text().set(it.loggerID);
    eventNode.append_child("msg").text().set(it.msg.c_str());
    eventNode.append_child("severity").text().set(it.severity);
  }

  // save xml
  return doc.save_file(xmlPath.c_str());
}

