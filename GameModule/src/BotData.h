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
	const std::string name;
	const int credit;
	const BotLanguage lang;
	const int numOfKnowledgeTables;
	const int* knowledgeTables;
	const std::string file;
	BotCommunicator* communicator;

	BotData(int id, std::string name, int credit, BotLanguage lang,
		int numOfKnowledgeTables, int* knowledgeTables, std::string file)
		: id(id), name(name), credit(credit), lang(lang),
		numOfKnowledgeTables(numOfKnowledgeTables), knowledgeTables(knowledgeTables),
		file(file)
	{}

	virtual ~BotData()
	{
		delete [] this->knowledgeTables;
	}
};

#endif // _BOTDATA_H