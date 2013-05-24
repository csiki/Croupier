#if !defined(_KNOWLEDGETABLE_H)
#define _KNOWLEDGETABLE_H

#include "KnowledgeDataType.h"
#include "AbstractColumn<T>.h"

class KnowledgeTable
{
private:
	vector<vector<void*> > data;
	int numOfRows;
	int numOfCols;
	KnowledgeDataType *columnTypes;
	AbstractColumn<T> *columns;
public:
	int addRow();
	bool deleteData(int row, int col);
	KnowledgeDatyType getColumnType(int col);
	T getData<T>(int row, int col);
	int getNumOfCols();
	int getNumOfRows();
	bool removeRow(int row);
	bool setData<T>(int T, int row, int col);
};

#endif  //_KNOWLEDGETABLE_H
