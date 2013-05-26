#if !defined(_BOTKNOWLEDGEHANDLER_H)
#define _BOTKNOWLEDGEHANDLER_H

#include "KnowledgeTable.h"
#include "KnowledgeDataType.h"

class BotKnowledgeHandler
{
private:
	int botID;
	KnowledgeTable* loadTable(int tableID);
	KnowledgeTable *loadedTables;
public:
	int addTableRow(int tableID);
	int createTable(int numOfCols, KnowledgeDataType* colTypes);
	bool deleteTableData(int tableID, int row, int col);
	KnowledgeTable* const getTable(int tableID);
	KnowledgeDataType getTableColumnType(int tableID, int col);
	T getTableData<T>(int tableID, int row, int col);
	int getTableNumOfCols(int tableID);
	int getTableNumOfRows(int tableID);
	bool removeTable(int tableID);
	bool removeTableRow(int tableID, int row);
	bool setTableData<T>(T data, int tableID, int row, int col);
};

#endif  //_BOTKNOWLEDGEHANDLER_H
