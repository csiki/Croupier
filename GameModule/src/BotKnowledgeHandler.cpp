#include "stdafx.h"
#include "BotKnowledgeHandler.h"
#include "KnowledgeTableXMLHandler.h"

/** Loads a knowledge table.
*/
KnowledgeTable* BotKnowledgeHandler::loadTable(int tableID)
{
	// TODO open xml with pugixml, using _BOT_KNOWLEDGE_RELATIVE_PATH_/botID/tableID.xml ...
	// use KnowledgeTableXMLHandler
	return 0;
}

/** Saves/serializes a knowledge table.
*/
void BotKnowledgeHandler::saveTable(KnowledgeTable* table)
{
	KnowledgeTableXMLHandler::saveXML(table, _BOT_KNOWLEDGE_RELATIVE_PATH_);
}

/** Returns if a knowledge table is loaded.
*/
bool BotKnowledgeHandler::isTableLoaded(int tableID) const
{
	map<int, KnowledgeTable*>::const_iterator it = this->loadedTables.find(tableID);
	
	if (it != this->loadedTables.end() && !it->second->isRemoved())
	{
		// can be found and not removed
		return true;
	}

	return false;
}

/** Adds a row to the specified table.
*/
int BotKnowledgeHandler::addTableRow(int tableID)
{
	if (this->isTableLoaded(tableID))
	{
		return this->loadedTables[tableID]->addRow();
	}

	return -1;
}

/** Creates an extra knowledge table
*/
int BotKnowledgeHandler::createTable(int numOfCols, KnowledgeDataType* colTypes)
{
	int desiredID = 1;

	for (map<int, KnowledgeTable*>::iterator it = this->loadedTables.begin(); it != this->loadedTables.end(); ++it)
	{
		if (it->first > desiredID)
		{
			desiredID = it->first;
		}
	}

	++desiredID; // max id of tables so far + 1

	KnowledgeTable* kt = new KnowledgeTable(numOfCols, colTypes, true); // updated=true (brand new table, should be updated)

	this->loadedTables.insert(pair<int, KnowledgeTable*>(desiredID, kt));

	return desiredID;
}

/** Returns type of the specified column in specified table.
*/
KnowledgeDataType BotKnowledgeHandler::getTableColumnType(int tableID, int col) const
{
	if (this->isTableLoaded(tableID))
	{
		return this->loadedTables.at(tableID)->getColumnType(col);
	}

	return KnowledgeDataType::NONE;
}

/** Returns knowledge table's data in specified cell.
*/
template <typename T>
T BotKnowledgeHandler::getTableData(int tableID, int row, int col) const
{
	// TODO
}

/** Sets data of a specified cell.
*/
template <typename T>
bool BotKnowledgeHandler::setTableData(T val, int tableID, int row, int col)
{
	// TODO
	return 0;
}

/** Returns number of columns of a specified table.
*/
int BotKnowledgeHandler::getTableNumOfCols(int tableID) const
{
	if (this->isTableLoaded(tableID))
	{
		return this->loadedTables.at(tableID)->getNumOfCols();
	}

	return -1;
}

/** Returns number of rows of a specified table.
*/
int BotKnowledgeHandler::getTableNumOfRows(int tableID) const
{
	if (this->isTableLoaded(tableID))
	{
		return this->loadedTables.at(tableID)->getNumOfRows();
	}

	return -1;
}

/** Removes a specified table permanently.
 *	On nth attempt to remove where n > 1, does nothing, return false.
 *	@return true if table found and has not been removed before, else false
*/
bool BotKnowledgeHandler::removeTable(int tableID)
{
	if (this->isTableLoaded(tableID))
	{
		this->loadedTables[tableID]->remove();
		return true;
	}

	return false;
}

/** Removes a specified row of a table permanently.
*/
bool BotKnowledgeHandler::removeTableRow(int tableID, int row)
{
	if (this->isTableLoaded(tableID))
	{
		return this->loadedTables[tableID]->removeRow(row);
	}

	return false;
}

