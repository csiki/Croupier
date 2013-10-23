#include "stdafx.h"
#include "LogXMLHandler.h"

/** Loads a Log object from an xml file.
*/
Log* LogXMLHandler::loadXML(std::string xmlPath)
{
	// load file, check if file exists
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlPath.c_str());

	Log* log = nullptr;
	if (result)
	{
		// file loaded successfully
		// create log object
		log = new Log();

		pugi::xpath_node logNode = doc.select_single_node("/log");

		// load events
		for (pugi::xml_node eventNode = logNode.node().child("event"); eventNode; eventNode = eventNode.next_sibling("event"))
		{
			int loggerID = eventNode.child("logger").text().as_int();
			const char* msg = eventNode.child("msg").text().as_string();
			Severity severity = static_cast<Severity>( eventNode.child("severity").text().as_int() );

			Event* event = new Event(severity, loggerID, msg);
			log->log(event);
		}
	}

	return log;
}

/** Saves a Log object as an xml file.
*/
bool LogXMLHandler::saveXML(Log* log, std::string xmlPath)
{
	if (log != nullptr)
	{
		pugi::xml_document doc;
		pugi::xml_node logNode = doc.append_child("log");

		// save events
		std::list<Event*>::const_iterator begin;
		std::list<Event*>::const_iterator end;
		log->getEventsIterators(begin, end);

		for (std::list<Event*>::const_iterator it = begin; it != end; ++it)
		{
			// create event node
			pugi::xml_node eventNode = logNode.append_child("event");

			// add nodes to event
			eventNode.append_child("logger").text().set( (*it)->loggerID );
			eventNode.append_child("msg").text().set( (*it)->msg.c_str() );
			eventNode.append_child("severity").text().set( (*it)->severity );
		}

		// save xml
		return doc.save_file(xmlPath.c_str());
	}

	return false;
}

