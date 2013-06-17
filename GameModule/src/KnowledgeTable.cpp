#include "stdafx.h"
#include "KnowledgeTable.h"

KnowledgeTable::KnowledgeTable(int numOfCols, KnowledgeDataType* colTypes, bool updated)
{
	// TODO
}

int KnowledgeTable::addRow()
{
	return 0;
}

KnowledgeDataType KnowledgeTable::getColumnType(int col) const
{
	if (this->numOfCols > col && col >= 0)
	{
		return this->columnTypes[col];
	}

	return KnowledgeDataType::NONE;
}


int KnowledgeTable::getNumOfCols() const
{
	return 0;
}

int KnowledgeTable::getNumOfRows() const
{
	return 0;
}

bool KnowledgeTable::isRemoved() const
{
	return 0;
}

void KnowledgeTable::remove()
{

}

bool KnowledgeTable::removeRow(int row)
{
	return 0;
}

bool KnowledgeTable::getData(int& val, int row, int col) const
{
	return 0;
}
bool KnowledgeTable::getData(bool& val, int row, int col) const
{
	return 0;
}
bool KnowledgeTable::getData(char& val, int row, int col) const
{
	return 0;
}
bool KnowledgeTable::getData(string& val, int row, int col) const
{
	return 0;
}
bool KnowledgeTable::getData(float& val, int row, int col) const
{
	return 0;
}

bool KnowledgeTable::setData(int val, int row, int col)
{
	return 0;
}
bool KnowledgeTable::setData(bool val, int row, int col)
{
	return 0;
}
bool KnowledgeTable::setData(char val, int row, int col)
{
	return 0;
}
bool KnowledgeTable::setData(string val, int row, int col)
{
	return 0;
}
bool KnowledgeTable::setData(float val, int row, int col)
{
	return 0;
}

