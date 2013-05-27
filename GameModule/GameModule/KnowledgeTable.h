#if !defined(_KNOWLEDGETABLE_H)
#define _KNOWLEDGETABLE_H

#include "KnowledgeDataType.h"
#include <vector>

using namespace std;

class KnowledgeTable
{
private:
	//int numOfRows; // in data
	//int numOfCols; // in data
	vector<vector<void*> > data;
	KnowledgeDataType *columnTypes;
public:
	int addRow();
	bool deleteData(int row, int col);
	KnowledgeDataType getColumnType(int col);
	int getNumOfCols();
	int getNumOfRows();
	bool removeRow(int row);
	template <typename T>
		T getData(int row, int col);
	template <typename T>
		bool setData(T val, int row, int col);
};

#endif  //_KNOWLEDGETABLE_H
