#include "stdafx.h"
#include "BotKnowledgeHandler.h"

KnowledgeTable* BotKnowledgeHandler::loadTable(int tableID)
{
	return 0;
}

int BotKnowledgeHandler::addTableRow(int tableID)
{
	return 0;
}

int BotKnowledgeHandler::createTable(int numOfCols, KnowledgeDataType* colTypes)
{
	return 0;
}

bool BotKnowledgeHandler::deleteTableData(int tableID, int row, int col)
{
	return 0;
}

KnowledgeTable* BotKnowledgeHandler::getTable(int tableID)
{
	return 0;
}

KnowledgeDataType BotKnowledgeHandler::getTableColumnType(int tableID, int col)
{
	return KnowledgeDataType::BOOL;
}

template <typename T>
T BotKnowledgeHandler::getTableData(int tableID, int row, int col)
{

}

template <typename T>
bool BotKnowledgeHandler::setTableData(T val, int tableID, int row, int col)
{
	return 0;
}

int BotKnowledgeHandler::getTableNumOfCols(int tableID)
{
	return 0;
}

int BotKnowledgeHandler::getTableNumOfRows(int tableID)
{
	return 0;
}

bool BotKnowledgeHandler::removeTable(int tableID)
{
	return 0;
}

bool BotKnowledgeHandler::removeTableRow(int tableID, int row)
{
	return 0;
}

