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
	const int numOfFiles;
	const std::string* files;
	BotCommunicator* communicator;

	BotData(int id, std::string name, int credit, BotLanguage lang,
		int numOfKnowledgeTables, int* knowledgeTables,
		int numOfFiles, std::string* files)
		: id(id), name(name), credit(credit), lang(lang),
		numOfKnowledgeTables(numOfKnowledgeTables), knowledgeTables(knowledgeTables),
		numOfFiles(numOfFiles), files(files)
	{}

	virtual ~BotData()
	{
		delete [] this->knowledgeTables;
		delete [] this->files;
	}
};

#endif // _BOTDATA_H