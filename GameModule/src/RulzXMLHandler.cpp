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
		
	}


	return rulz;
}

/** Saves a Rulz object as an xml file.
*/
bool RulzXMLHandler::saveXML(Rulz* log, string xmlPath)
{
	return false;
}

