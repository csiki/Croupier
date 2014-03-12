#if !defined(_KNOWLEDGETABLE_H)
#define _KNOWLEDGETABLE_H

#include "KnowledgeDataType.h"

/**	Table for containing AIs' own permanent knowledge.
*/
class KnowledgeTable
{
private:
	std::vector<std::vector<void*> > data;
	size_t numOfCols;
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

	virtual ~KnowledgeTable() // UNIT
	{
		// deallocate data
		for (unsigned int i = 0; i < data.size(); ++i)
		{
			for (unsigned int j = 0; j < data[i].size(); ++j)
			{
				// cast to type
				switch (this->columnTypes[j])
				{
					case KnowledgeDataType::BOOL:
					{
						bool* d = (bool*) data[i][j];
						delete d;
						break;
					}
					case KnowledgeDataType::CHAR:
					{
						char* d = (char*) data[i][j];
						delete d;
						break;
					}
					case KnowledgeDataType::FLOAT:
					{
						float* d = (float*) data[i][j];
						delete d;
						break;
					}
					case KnowledgeDataType::INT:
					{
						int* d = (int*) data[i][j];
						delete d;
						break;
					}
					case KnowledgeDataType::STRING:
					{
						std::string* d = (std::string*) data[i][j];
						delete d;
						break;
					}
					default: ;
						// error occured
						//delete data[i][j];
				}
			}
		}

		// deallocate column types
		delete [] this->columnTypes;
	}

	int addRow(); // UNIT done
	KnowledgeDataType getColumnType(int col) const;
	int getNumOfCols() const;
	int getNumOfRows() const;
	bool isUpdated() const;
	bool isRemoved() const;
	void remove();
	bool removeRow(int row); // UNIT done

	bool getData(int& val, int row, int col) const;
	bool getData(bool& val, int row, int col) const; // UNIT done
	bool getData(char& val, int row, int col) const;
	bool getData(std::string& val, int row, int col) const;
	bool getData(float& val, int row, int col) const;

	bool setData(int val, int row, int col);
	bool setData(bool val, int row, int col); // UNIT done
	bool setData(char val, int row, int col);
	bool setData(std::string val, int row, int col);
	bool setData(float val, int row, int col);
};

#endif  //_KNOWLEDGETABLE_H
