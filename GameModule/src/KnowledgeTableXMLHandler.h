#if !defined(_KNOWLEDGETABLEXMLHANDLER_H)
#define _KNOWLEDGETABLEXMLHANDLER_H

#include "KnowledgeTable.h"

class KnowledgeTableXMLHandler {
public:
	static KnowledgeTable* loadXML(const char* xmlpath);
	static bool saveXML(KnowledgeTable* kt, const char* xmlpath);
};

#endif  //_KNOWLEDGETABLEXMLHANDLER_H
