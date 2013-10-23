#if !defined(_BOTKNOWLEDGEHANDLER_H)
#define _BOTKNOWLEDGEHANDLER_H

#include "KnowledgeTable.h"
#include "KnowledgeDataType.h"
#include "KnowledgeTableXMLHandler.h"
#include "../pugixml/pugixml.hpp"
#include "BotData.h"

/**	Class for handling AIs' permanent knowledge.
*/
class BotKnowledgeHandler
{
private:
	const int userID;
	std::string relPath;
	std::map<int, KnowledgeTable*> loadedTables;

	KnowledgeTable* loadTable(int tableID);
	void saveTable(int tableID, KnowledgeTable* table);
	void removeTableFile(int tableID);
public:

	BotKnowledgeHandler(int userID, int numOfKnowledgeTables, const int* knowledgeTables) : userID(userID)
	{
		// set relPath
		this->relPath = _BOT_KNOWLEDGE_RELATIVE_PATH_;
		this->relPath += std::to_string(this->userID);
		this->relPath += "/";

		// load tables
		for (size_t i = 0; i < numOfKnowledgeTables; ++i)
		{
			KnowledgeTable* kt = this->loadTable(knowledgeTables[i]);

			if (kt != nullptr)
			{
				this->loadedTables.insert( std::pair<int, KnowledgeTable*>(knowledgeTables[i], kt) );
			}
		}
	}

	virtual ~BotKnowledgeHandler()
	{
		// save tables
		for (std::map<int, KnowledgeTable*>::iterator it = this->loadedTables.begin(); it != this->loadedTables.end(); ++it)
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

	bool isTableLoaded(int tableID) const;
	int addTableRow(int tableID);
	int createTable(int numOfCols, KnowledgeDataType* colTypes); // UNIT done
	KnowledgeDataType getTableColumnType(int tableID, int col) const;
	int getTableNumOfCols(int tableID) const;
	int getTableNumOfRows(int tableID) const;
	bool removeTable(int tableID); // UNIT done
	bool removeTableRow(int tableID, int row); // UNIT done
	
	bool getTableData(int& val, int tableID, int row, int col) const;
	bool getTableData(bool& val, int tableID, int row, int col) const; // UNIT done
	bool getTableData(char& val, int tableID, int row, int col) const;
	bool getTableData(float& val, int tableID, int row, int col) const;
	bool getTableData(std::string& val, int tableID, int row, int col) const;

	bool setTableData(int val, int tableID, int row, int col);
	bool setTableData(bool val, int tableID, int row, int col); // UNIT done
	bool setTableData(char val, int tableID, int row, int col);
	bool setTableData(float val, int tableID, int row, int col);
	bool setTableData(std::string val, int tableID, int row, int col);
};

#endif  //_BOTKNOWLEDGEHANDLER_H
