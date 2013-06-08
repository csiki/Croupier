#if !defined(_KNOWLEDGETABLE_H)
#define _KNOWLEDGETABLE_H

#include "KnowledgeDataType.h"
#include <vector>

using namespace std;

/**	Table for containing AIs' own permanent knowledge.
*/
class KnowledgeTable
{
private:
	vector<vector<void*> > data;
	KnowledgeDataType *columnTypes;
public:
	int addRow();
	KnowledgeDataType getColumnType(int col) const;
	int getNumOfCols() const;
	int getNumOfRows() const;
	bool removeRow(int row);
	template <typename T>
		T getData(int row, int col) const;
	template <typename T>
		bool setData(T val, int row, int col);
};

#endif  //_KNOWLEDGETABLE_H
