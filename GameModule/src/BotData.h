#if !defined(_BOTDATA_H)
#define _BOTDATA_H

#include "BotLanguage.h"
#include "BotTimeExceededException.h"

class BotCommunicator;

/** Represents a bot before loaded.
*/
class BotData
{
public:
	const int id;
	const int playerID;
	const std::string name;
	const int credit;
	const BotLanguage lang;
	const int numOfKnowledgeTables;
	const int* knowledgeTables;
	const BotCommunicator* communicator;

	BotData(int id, int playerID, std::string name, int credit, BotLanguage lang,
		int numOfKnowledgeTables, int* knowledgeTables)
		: id(id), playerID(playerID), name(name), credit(credit), lang(lang),
		numOfKnowledgeTables(numOfKnowledgeTables), knowledgeTables(knowledgeTables)
	{}

	virtual ~BotData()
	{
		delete [] this->knowledgeTables;
	}
};

#endif // _BOTDATA_H