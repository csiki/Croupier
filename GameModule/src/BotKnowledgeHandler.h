#if !defined(_BOTKNOWLEDGEHANDLER_H)
#define _BOTKNOWLEDGEHANDLER_H

#include "KnowledgeTable.h"
#include "KnowledgeDataType.h"
#include "KnowledgeTableXMLHandler.h"
#include "../pugixml/pugixml.hpp"

// TODO át kell írni a analízis doksiba, hogy a getTable...() parancsok egyike sem tölt be új táblát,
// a játék elején betöltjük a botok által requestelt összes táblát

// TODO neki a feladata h a knowledge tableöket mentse és töltse

/**	Class for handling AIs' permanent knowledge.
*/
class BotKnowledgeHandler
{
private:
	const int userID;
	string relPath;
	map<int, KnowledgeTable*> loadedTables;

	KnowledgeTable* loadTable(int tableID);
	void saveTable(int tableID, KnowledgeTable* table);
	void removeTableFile(int tableID);
	bool isTableLoaded(int tableID) const;
public:

	BotKnowledgeHandler(int userID, int numOfRequestedTable, int* requestedTables) : userID(userID)
	{
		relPath = _BOT_KNOWLEDGE_RELATIVE_PATH_;
		relPath += this->userID;
		relPath += "/";

		// load tables
		for (int i = 0; i < numOfRequestedTable; ++i)
		{
			KnowledgeTable* kt = this->loadTable(requestedTables[i]);

			if (kt != nullptr)
			{
				this->loadedTables.insert( pair<int, KnowledgeTable*>(requestedTables[i], kt) );
			}
		}
	}

	virtual ~BotKnowledgeHandler()
	{
		// save tables
		for (map<int, KnowledgeTable*>::iterator it = this->loadedTables.begin(); it != this->loadedTables.end(); ++it)
		{
			if (it->second->isRemoved())
			{
				this->removeTable(it->first);
			}
			else if (it->second->isUpdated())
			{
				this->saveTable(it->first, it->second);
			}

			delete it->second;
		}
	}

	int addTableRow(int tableID);
	int createTable(int numOfCols, KnowledgeDataType* colTypes);
	KnowledgeDataType getTableColumnType(int tableID, int col) const;
	int getTableNumOfCols(int tableID) const;
	int getTableNumOfRows(int tableID) const;
	bool removeTable(int tableID);
	bool removeTableRow(int tableID, int row);
	
	bool getTableData(int& val, int tableID, int row, int col) const;
	bool getTableData(bool& val, int tableID, int row, int col) const;
	bool getTableData(char& val, int tableID, int row, int col) const;
	bool getTableData(float& val, int tableID, int row, int col) const;
	bool getTableData(string& val, int tableID, int row, int col) const;

	bool setTableData(int val, int tableID, int row, int col);
	bool setTableData(bool val, int tableID, int row, int col);
	bool setTableData(char val, int tableID, int row, int col);
	bool setTableData(float val, int tableID, int row, int col);
	bool setTableData(string val, int tableID, int row, int col);
};

#endif  //_BOTKNOWLEDGEHANDLER_H
