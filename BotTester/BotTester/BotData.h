#if !defined(_BOTDATA_H)
#define _BOTDATA_H

#include "BotLanguage.h"
#include <string>

class BotCommunicator;

/** Represents a bot before loaded.
*/
class BotData
{
public:
	const int id;
	const int playerID;
	const std::string name;
	const std::string src;
	const int credit;
	const BotLanguage lang;
	const int numOfKnowledgeTables;
	const int* knowledgeTables;
	BotCommunicator* communicator;

	BotData(int id, int playerID, std::string name, std::string src, int credit, BotLanguage lang,
		int numOfKnowledgeTables, int* knowledgeTables)
		: id(id), playerID(playerID), name(name), src(src), credit(credit), lang(lang),
		numOfKnowledgeTables(numOfKnowledgeTables), knowledgeTables(knowledgeTables)
	{}

	virtual ~BotData()
	{
	    if (numOfKnowledgeTables > 0)
        {
            delete [] this->knowledgeTables;
        }
	}
};

#endif // _BOTDATA_H
