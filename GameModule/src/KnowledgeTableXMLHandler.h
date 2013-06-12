#if !defined(_KNOWLEDGETABLEXMLHANDLER_H)
#define _KNOWLEDGETABLEXMLHANDLER_H

#include "KnowledgeTable.h"

/**	Loads and saves AI's permanent data in xml format.
*/
class KnowledgeTableXMLHandler {
public:
	static KnowledgeTable* loadXML(const char* xmlpath);
	static bool saveXML(KnowledgeTable* kt, const char* xmlpath);
};

#endif  //_KNOWLEDGETABLEXMLHANDLER_H
