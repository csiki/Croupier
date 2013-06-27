#include "stdafx.h"
#include "KnowledgeTable.h"

KnowledgeTable::KnowledgeTable(int numOfCols, KnowledgeDataType* colTypes, bool updated)
{
	// TODO
}

/** Returns if a cell exists in table.
*/
bool KnowledgeTable::isCellExist(int row, int col) const
{
	return row >= 0 && col >=0 && col < this->numOfCols && row < this->getNumOfRows();
}

/** Returns if the given data type is the same at the given column.
*/
bool KnowledgeTable::isDataTypeCorrect(int col, KnowledgeDataType kdt) const
{
	if (col >= 0 && col < this->numOfCols)
	{
		return this->columnTypes[col] == kdt;
	}

	return false;
}

/** Adds a row to table.
*/
int KnowledgeTable::addRow()
{
	vector<void*> row(this->numOfCols);
	this->data.push_back(row);
	
	return this->data.size() - 1;
}

/** Returns type of the given column.
*/
KnowledgeDataType KnowledgeTable::getColumnType(int col) const
{
	if (col >= 0 && col < this->numOfCols)
	{
		return this->columnTypes[col];
	}

	return KnowledgeDataType::NONE;
}

/** Returns number of the columns in table.
*/
int KnowledgeTable::getNumOfCols() const
{
	return this->numOfCols;
}

/** Returns number of the rows in table.
*/
int KnowledgeTable::getNumOfRows() const
{
	return this->data.size();
}

/** Returns if a table is removed (deleted) by the bot, during the game.
*/
bool KnowledgeTable::isRemoved() const
{
	return this->removed;
}

/** Sets the removed flag (doesn't remove it).
*/
void KnowledgeTable::remove()
{
	this->removed = true;
}

/** Removes a row from the table.
*/
bool KnowledgeTable::removeRow(int row)
{
	if (this->isCellExist(row, 0))
	{
		this->data.erase(this->data.begin() + row);
		return true;
	}

	return false;
}

/** Gets data from table at given row and col (int).
*/
bool KnowledgeTable::getData(int& val, int row, int col) const
{
	if (this->isCellExist(row, col) && this->isDataTypeCorrect(col, KnowledgeDataType::INT))
	{
		val = *((int*) this->data[row].at(col));
		return true;
	}

	return false;
}

/** Gets data from table at given row and col (bool).
*/
bool KnowledgeTable::getData(bool& val, int row, int col) const
{
	if (this->isCellExist(row, col) && this->isDataTypeCorrect(col, KnowledgeDataType::BOOL))
	{
		val = *((bool*) this->data[row].at(col));
		return true;
	}

	return false;
}

/** Gets data from table at given row and col (char).
*/
bool KnowledgeTable::getData(char& val, int row, int col) const
{
	if (this->isCellExist(row, col) && this->isDataTypeCorrect(col, KnowledgeDataType::CHAR))
	{
		val = *((char*) this->data[row].at(col));
		return true;
	}

	return false;
}

/** Gets data from table at given row and col (string).
*/
bool KnowledgeTable::getData(string& val, int row, int col) const
{
	if (this->isCellExist(row, col) && this->isDataTypeCorrect(col, KnowledgeDataType::STRING))
	{
		val = *((string*) this->data[row].at(col));
		return true;
	}

	return false;
}

/** Gets data from table at given row and col (float).
*/
bool KnowledgeTable::getData(float& val, int row, int col) const
{
	if (this->isCellExist(row, col) && this->isDataTypeCorrect(col, KnowledgeDataType::FLOAT))
	{
		val = *((float*) this->data[row].at(col));
		return true;
	}

	return false;
}

/** Sets data int table at given row and col (int).
*/
bool KnowledgeTable::setData(int val, int row, int col)
{
	if (this->isCellExist(row, col) && this->isDataTypeCorrect(col, KnowledgeDataType::INT))
	{
		int* p = new int(val);
		this->data[row][col] = p;
	}

	return false;
}

/** Sets data int table at given row and col (bool).
*/
bool KnowledgeTable::setData(bool val, int row, int col)
{
	if (this->isCellExist(row, col) && this->isDataTypeCorrect(col, KnowledgeDataType::BOOL))
	{
		bool* p = new bool(val);
		this->data[row][col] = p;
	}

	return false;
}

/** Sets data int table at given row and col (char).
*/
bool KnowledgeTable::setData(char val, int row, int col)
{
	if (this->isCellExist(row, col) && this->isDataTypeCorrect(col, KnowledgeDataType::CHAR))
	{
		char* p = new char(val);
		this->data[row][col] = p;
	}

	return false;
}

/** Sets data int table at given row and col (string).
*/
bool KnowledgeTable::setData(string val, int row, int col)
{
	if (this->isCellExist(row, col) && this->isDataTypeCorrect(col, KnowledgeDataType::STRING))
	{
		string* p = new string(val);
		this->data[row][col] = p;
	}

	return false;
}

/** Sets data int table at given row and col (float).
*/
bool KnowledgeTable::setData(float val, int row, int col)
{
	if (this->isCellExist(row, col) && this->isDataTypeCorrect(col, KnowledgeDataType::FLOAT))
	{
		float* p = new float(val);
		this->data[row][col] = p;
	}

	return false;
}

