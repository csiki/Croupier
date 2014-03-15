#if !defined(_KNOWLEDGETABLEXMLHANDLER_H)
#define _KNOWLEDGETABLEXMLHANDLER_H

#include "KnowledgeTable.h"
#include "pugixml.hpp"

/**	Loads and saves AI's permanent data in xml format.
*/
class KnowledgeTableXMLHandler {
public:
	static KnowledgeTable* loadXML(std::string xmlpath);
	static bool saveXML(KnowledgeTable* kt, std::string xmlpath);
};

#endif  //_KNOWLEDGETABLEXMLHANDLER_H
