#if !defined(_BOTKNOWLEDGEHANDLER_H)
#define _BOTKNOWLEDGEHANDLER_H

#include "KnowledgeTable.h"
#include "KnowledgeDataType.h"
#include "../pugixml/pugixml.hpp"

// TODO át kell írni a analízis doksiba, hogy a getTable...() parancsok egyike sem tölt be új táblát,
// a játék elején betöltjük a botok által requestelt összes táblát

/**	Class for handling AIs' permanent knowledge.
*/
class BotKnowledgeHandler
{
private:
	const int botID;
	map<int, KnowledgeTable*> loadedTables;

	KnowledgeTable* loadTable(int tableID);
	void saveTable(KnowledgeTable* table);
	bool isTableLoaded(int tableID) const;
public:
	// konstruktorba töltjük be a táblákat ! konstruktor paraméterei legalább: botID, numofRequestedTable:int, requestedTables: int*
	int addTableRow(int tableID);
	int createTable(int numOfCols, KnowledgeDataType* colTypes);
	KnowledgeDataType getTableColumnType(int tableID, int col) const;
	int getTableNumOfCols(int tableID) const;
	int getTableNumOfRows(int tableID) const;
	bool removeTable(int tableID);
	bool removeTableRow(int tableID, int row);
	template <typename T>
		T getTableData(int tableID, int row, int col) const;
	template <typename T>
		bool setTableData(T val, int tableID, int row, int col);
};

#endif  //_BOTKNOWLEDGEHANDLER_H
