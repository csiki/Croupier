#if !defined(_BOTDATA_H)
#define _BOTDATA_H

#include "BotLanguage.h"

/** Represents a bot before loaded.
*/
class BotData
{
public:
	const int id;
	const BotLanguage lang;
	const int numOfKnowledgeTables;
	const int* knowledgeTables;
	const int numOfFiles;
	const string* files;

	BotData(int id, BotLanguage lang,
		int numOfKnowledgeTables, int* knowledgeTables,
		int numOfFiles, string* files)
		: id(id), lang(lang),
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