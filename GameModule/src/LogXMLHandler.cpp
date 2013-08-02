#include "stdafx.h"
#include "LogXMLHandler.h"

/** Loads a Log object from an xml file.
*/
Log* LogXMLHandler::loadXML(string xmlPath)
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
			Severity severity = (Severity) eventNode.child("severity").text().as_int();

			Event* event = new Event(severity, loggerID, msg);
			log->log(event);
		}
	}

	return log;
}

/** Saves a Log object as an xml file.
*/
bool LogXMLHandler::saveXML(Log* log, string xmlPath)
{
	if (log != nullptr)
	{
		pugi::xml_document doc;
		pugi::xml_node logNode = doc.append_child("log");
		
		// save events
		list<Event*>::iterator begin;
		list<Event*>::iterator end;
		log->getEventsIterators(begin, end);

		for (list<Event*>::iterator it = begin; it != end; ++it)
		{
			// create event node
			pugi::xml_node eventNode = logNode.append_child("event");

			// add nodes to event
			pugi::xml_node loggerNode = eventNode.append_child("logger");
			pugi::xml_node msgNode = eventNode.append_child("msg");
			pugi::xml_node sevNode = eventNode.append_child("severity");

			// fill nodes
			loggerNode.text().set( (*it)->loggerID );
			msgNode.text().set( (*it)->msg.c_str() );
			sevNode.text().set( (*it)->severity );
		}
		
		// save xml
		return doc.save_file(xmlPath.c_str());
	}

	return false;
}

