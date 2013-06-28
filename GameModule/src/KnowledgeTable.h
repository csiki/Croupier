#if !defined(_KNOWLEDGETABLE_H)
#define _KNOWLEDGETABLE_H

#include "KnowledgeDataType.h"

using namespace std;

/**	Table for containing AIs' own permanent knowledge.
*/
class KnowledgeTable
{
private:
	vector<vector<void*> > data;
	int numOfCols;
	KnowledgeDataType *columnTypes;
	bool updated;
	bool removed;

	bool isCellExist(int row, int col) const;
	bool isDataTypeCorrect(int col, KnowledgeDataType kdt) const;
public:

	KnowledgeTable(int numOfCols, KnowledgeDataType* colTypes, bool updated = false)
	{
		this->updated = false;
		this->removed = false;
		this->numOfCols = numOfCols;
		this->columnTypes = colTypes;
	}

	int addRow();
	KnowledgeDataType getColumnType(int col) const;
	int getNumOfCols() const;
	int getNumOfRows() const;
	bool isRemoved() const;
	void remove();
	bool removeRow(int row);

	bool getData(int& val, int row, int col) const;
	bool getData(bool& val, int row, int col) const;
	bool getData(char& val, int row, int col) const;
	bool getData(string& val, int row, int col) const;
	bool getData(float& val, int row, int col) const;

	bool setData(int val, int row, int col);
	bool setData(bool val, int row, int col);
	bool setData(char val, int row, int col);
	bool setData(string val, int row, int col);
	bool setData(float val, int row, int col);
};

#endif  //_KNOWLEDGETABLE_H
