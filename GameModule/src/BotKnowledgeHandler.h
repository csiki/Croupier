#if !defined(_BOTKNOWLEDGEHANDLER_H)
#define _BOTKNOWLEDGEHANDLER_H

#include "KnowledgeTable.h"
#include "KnowledgeDataType.h"
#include "../pugixml/pugixml.hpp"

// TODO �t kell �rni a anal�zis doksiba, hogy a getTable...() parancsok egyike sem t�lt be �j t�bl�t,
// a j�t�k elej�n bet�ltj�k a botok �ltal requestelt �sszes t�bl�t

/**	Class for handling AIs' permanent knowledge.
*/
class BotKnowledgeHandler
{
private:
	const int userID;
	map<int, KnowledgeTable*> loadedTables;

	KnowledgeTable* loadTable(int tableID);
	void saveTable(int tableID, KnowledgeTable* table);
	bool isTableLoaded(int tableID) const;
public:
	// konstruktorba t�ltj�k be a t�bl�kat ! konstruktor param�terei legal�bb: userID, numofRequestedTable:int, requestedTables: int*
	int addTableRow(int tableID);
	int createTable(int numOfCols, KnowledgeDataType* colTypes);
	KnowledgeDataType getTableColumnType(int tableID, int col) const;
	int getTableNumOfCols(int tableID) const;
	int getTableNumOfRows(int tableID) const;
	bool removeTable(int tableID);
	bool removeTableRow(int tableID, int row);
	template <typename T>
		T getTableData(int tableID, int row, int col, bool* error) const;
	template <typename T>
		void setTableData(T val, int tableID, int row, int col, bool* error);
};

#endif  //_BOTKNOWLEDGEHANDLER_H
