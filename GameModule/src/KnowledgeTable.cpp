#include "stdafx.h"
#include "KnowledgeTable.h"

int KnowledgeTable::addRow()
{
	return 0;
}

KnowledgeDataType KnowledgeTable::getColumnType(int col) const
{
	return KnowledgeDataType::BOOL;
}


int KnowledgeTable::getNumOfCols() const
{
	return 0;
}

int KnowledgeTable::getNumOfRows() const
{
	return 0;
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

