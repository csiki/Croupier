#if !defined(_BOTKNOWLEDGEHANDLER_H)
#define _BOTKNOWLEDGEHANDLER_H

#include "KnowledgeTable.h"
#include "KnowledgeDataType.h"

class BotKnowledgeHandler
{
private:
	int botID;
	KnowledgeTable** loadedTables;

	KnowledgeTable* loadTable(int tableID);
public:
	int addTableRow(int tableID);
	int createTable(int numOfCols, KnowledgeDataType* colTypes);
	bool deleteTableData(int tableID, int row, int col);
	KnowledgeTable* getTable(int tableID);
	KnowledgeDataType getTableColumnType(int tableID, int col);
	int getTableNumOfCols(int tableID);
	int getTableNumOfRows(int tableID);
	bool removeTable(int tableID);
	bool removeTableRow(int tableID, int row);
	template <typename T>
		T getTableData(int tableID, int row, int col);
	template <typename T>
		bool setTableData(T val, int tableID, int row, int col);
};

#endif  //_BOTKNOWLEDGEHANDLER_H
