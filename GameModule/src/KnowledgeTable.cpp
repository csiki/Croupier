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

template <typename T>
T KnowledgeTable::getData(int row, int col) const
{

}

template <typename T>
bool KnowledgeTable::setData(T val, int row, int col)
{
	return 0;
}

