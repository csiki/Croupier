#include "stdafx.h"
#include "BotKnowledgeHandler.h"

/** Loads a knowledge table.
*/
KnowledgeTable* BotKnowledgeHandler::loadTable(int tableID)
{
	string path = _BOT_KNOWLEDGE_RELATIVE_PATH_;
	path += this->userID;
	path += "/";
	path += tableID;
	path += ".xml";

	return KnowledgeTableXMLHandler::loadXML(path);
}

/** Saves/serializes a knowledge table.
*/
void BotKnowledgeHandler::saveTable(int tableID, KnowledgeTable* table)
{
	string path = _BOT_KNOWLEDGE_RELATIVE_PATH_;
	path += this->userID;
	path += "/";
	path += tableID;
	path += ".xml";

	KnowledgeTableXMLHandler::saveXML(table, path);
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

/** Returns knowledge table's data in specified cell (int).
*/
bool BotKnowledgeHandler::getTableData(int& val, int tableID, int row, int col) const
{
	if (this->isTableLoaded(tableID))
	{
		return this->loadedTables.at(tableID)->getData(val, row, col);
	}

	return false;
}

/** Returns knowledge table's data in specified cell (bool).
*/
bool BotKnowledgeHandler::getTableData(bool& val, int tableID, int row, int col) const
{
	if (this->isTableLoaded(tableID))
	{
		return this->loadedTables.at(tableID)->getData(val, row, col);
	}

	return false;
}

/** Returns knowledge table's data in specified cell (char).
*/
bool BotKnowledgeHandler::getTableData(char& val, int tableID, int row, int col) const
{
	if (this->isTableLoaded(tableID))
	{
		return this->loadedTables.at(tableID)->getData(val, row, col);
	}

	return false;
}

/** Returns knowledge table's data in specified cell (float).
*/
bool BotKnowledgeHandler::getTableData(float& val, int tableID, int row, int col) const
{
	if (this->isTableLoaded(tableID))
	{
		return this->loadedTables.at(tableID)->getData(val, row, col);
	}

	return false;
}

/** Returns knowledge table's data in specified cell (std::string).
*/
bool BotKnowledgeHandler::getTableData(std::string& val, int tableID, int row, int col) const
{
	if (this->isTableLoaded(tableID))
	{
		return this->loadedTables.at(tableID)->getData(val, row, col);
	}

	return false;
}

/** Sets data of a specified cell (int).
*/
bool BotKnowledgeHandler::setTableData(int val, int tableID, int row, int col)
{
	if (this->isTableLoaded(tableID))
	{
		return this->loadedTables.at(tableID)->setData(val, row, col);
	}

	return false;
}

/** Sets data of a specified cell (bool).
*/
bool BotKnowledgeHandler::setTableData(bool val, int tableID, int row, int col)
{
	if (this->isTableLoaded(tableID))
	{
		return this->loadedTables.at(tableID)->setData(val, row, col);
	}

	return false;
}

/** Sets data of a specified cell (char).
*/
bool BotKnowledgeHandler::setTableData(char val, int tableID, int row, int col)
{
	if (this->isTableLoaded(tableID))
	{
		return this->loadedTables.at(tableID)->setData(val, row, col);
	}

	return false;
}

/** Sets data of a specified cell (float).
*/
bool BotKnowledgeHandler::setTableData(float val, int tableID, int row, int col)
{
	if (this->isTableLoaded(tableID))
	{
		return this->loadedTables.at(tableID)->setData(val, row, col);
	}

	return false;
}

/** Sets data of a specified cell (std::string).
*/
bool BotKnowledgeHandler::setTableData(std::string val, int tableID, int row, int col)
{
	if (this->isTableLoaded(tableID))
	{
		return this->loadedTables.at(tableID)->setData(val, row, col);
	}

	return false;
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

