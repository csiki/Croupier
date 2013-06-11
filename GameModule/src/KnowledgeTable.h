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
public:
	KnowledgeTable(int numOfCols, KnowledgeDataType* colTypes, bool updated = false);

	int addRow();
	KnowledgeDataType getColumnType(int col) const;
	int getNumOfCols() const;
	int getNumOfRows() const;
	bool isRemoved() const;
	void remove();
	bool removeRow(int row);
	template <typename T>
		T getData(int row, int col) const;
	template <typename T>
		bool setData(T val, int row, int col);
};

#endif  //_KNOWLEDGETABLE_H
