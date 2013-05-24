#include "BotKnowledgeHandler.h"
#include "KnowledgeDataType.h"

KnowledgeTable* BotKnowledgeHandler::loadTable(int tableID)
{

}

int BotKnowledgeHandler::addTableRow(int tableID)
{

}

int BotKnowledgeHandler::createTable(int numOfCols, KnowledgeDataType* colTypes)
{

}

bool BotKnowledgeHandler::deleteTableData(int tableID, int row, int col)
{

}

KnowledgeTable* const BotKnowledgeHandler::getTable(int tableID)
{

}

KnowledgeDataType BotKnowledgeHandler::getTableColumnType(int tableID, int col)
{

}

T BotKnowledgeHandler::getTableData<T>(int tableID, int row, int col)
{

}

int BotKnowledgeHandler::getTableNumOfCols(int tableID)
{

}

int BotKnowledgeHandler::getTableNumOfRows(int tableID)
{

}

bool BotKnowledgeHandler::removeTable(int tableID)
{

}

bool BotKnowledgeHandler::removeTableRow(int tableID, int row)
{

}

bool BotKnowledgeHandler::setTableData<T>(T data, int tableID, int row, int col)
{

}

