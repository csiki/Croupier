#if !defined(_BOTKNOWLEDGEHANDLER_H)
#define _BOTKNOWLEDGEHANDLER_H

#include "KnowledgeTable.h"
#include "KnowledgeDataType.h"

// TODO �t kell �rni a anal�zis doksiba, hogy a getTable...() parancsok egyike sem t�lt be �j t�bl�t,
// a j�t�k elej�n bet�ltj�k a botok �ltal requestelt �sszes t�bl�t (mondjuk egy mindig bet�lt�tt t�bl�ban tarthatj�k sz�mon ha �j t�bl�t k�sz�tettek j�t�k k�zben - kva r�szletk�rd�s)

/**	Class for handling AIs' permanent knowledge.
*/
class BotKnowledgeHandler
{
private:
	const int botID;
	int numOfTables;
	KnowledgeTable** loadedTables;

	KnowledgeTable* loadTable(int tableID);
public:
	// konstruktorba t�ltj�k be a t�bl�kat ! konstruktor param�terei legal�bb: numofRequestedTable:int, requestedTables: int*
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
