#include "stdafx.h"
#include "KnowledgeTableXMLHandler.h"

/** From an xml file, loads a KnowledgeTable instance.
*/
KnowledgeTable* KnowledgeTableXMLHandler::loadXML(std::string xmlPath)
{
	// load file, check if file exists
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlPath.c_str());

	KnowledgeTable* ktable = nullptr;
	if (result)
	{
		// file loaded successfully
		// iterate through column types
		pugi::xpath_node columnTypes = doc.select_single_node("/table/thead");
		std::list<KnowledgeDataType> ctypes;
		
		for (pugi::xml_node colType = columnTypes.node().child("coltype"); colType; colType = colType.next_sibling("coltype"))
		{
			KnowledgeDataType ctype = static_cast<KnowledgeDataType>( colType.text().as_int() );
			ctypes.push_back(ctype);
		}

		// create KnowledgeTable, iterate through data
		if (ctypes.size() > 0)
		{
			KnowledgeDataType* ctypesArray = new KnowledgeDataType[ctypes.size()];
			
			// copy column types to array
			int i = 0;
			for (std::list<KnowledgeDataType>::iterator it = ctypes.begin(); it != ctypes.end(); ++it)
			{
				ctypesArray[i] = *it;
				++i;
			}

			ktable = new KnowledgeTable(ctypes.size(), ctypesArray);
			// ctypesArray must be deleted in KnowledgeTable destructor

			// load data from xml
			pugi::xpath_node data = doc.select_single_node("/table/tbody");
			for (pugi::xml_node row = data.node().child("tr"); row; row = row.next_sibling("tr"))
			{
				// add new row, fill cells
				int rowindex = ktable->addRow();
				int colindex = 0;
				for (pugi::xml_node cell = row.child("td"); cell; cell = cell.next_sibling("td"))
				{
					switch (ctypesArray[colindex])
					{
					case KnowledgeDataType::BOOL:
						ktable->setData(cell.text().as_bool(), rowindex, colindex);
						break;
					case KnowledgeDataType::CHAR:
						ktable->setData(cell.text().as_string()[0], rowindex, colindex);
						break;
					case KnowledgeDataType::FLOAT:
						ktable->setData(cell.text().as_float(), rowindex, colindex);
						break;
					case KnowledgeDataType::INT:
						ktable->setData(cell.text().as_int(), rowindex, colindex);
						break;
					case KnowledgeDataType::STRING:
					{
						std::string str(cell.text().as_string());
						ktable->setData(str, rowindex, colindex);
						break;
					}
					default:
						// error occured
						delete ktable;
						return nullptr;
						break;
					}

					++colindex;
				}
			}
		}
	}

	return ktable;
}

/** Saves a KnowledgeTable instance as an xml file.
*/
bool KnowledgeTableXMLHandler::saveXML(KnowledgeTable* kt, std::string xmlPath)
{
	if (kt != nullptr)
	{
		pugi::xml_document doc;
		
		pugi::xml_node tableNode = doc.append_child("table");
		pugi::xml_node theadNode = tableNode.append_child("thead");
		pugi::xml_node tbodyNode = tableNode.append_child("tbody");

		// saving column types
		for (int i = 0; i < kt->getNumOfCols(); ++i)
		{
			KnowledgeDataType ctype = kt->getColumnType(i);
			pugi::xml_node ctypeNode = theadNode.append_child("coltype");
			
			int ctypeInt = ctype;
			char str[2];
			str[0] = '0' + ctype;
			str[1] = '\0';

			ctypeNode.text().set(str);
		}

		// saving data
		for (int rindex = 0; rindex < kt->getNumOfRows(); ++rindex)
		{
			pugi::xml_node rowNode = tbodyNode.append_child("tr");
			for (int cindex = 0; cindex < kt->getNumOfCols(); ++cindex)
			{
				pugi::xml_node cellNode = rowNode.append_child("td");
				KnowledgeDataType dataType = kt->getColumnType(cindex);

				if (dataType == KnowledgeDataType::BOOL)
				{
					bool d;
					if (kt->getData(d, rindex, cindex))
					{
						cellNode.text().set(d);
					}
					else
					{
						return false;
					}
				}
				else if (dataType == KnowledgeDataType::CHAR)
				{
					char d;
					char str[2];

					if (kt->getData(d, rindex, cindex))
					{
						str[0] = d;
						str[1] = '\0';
						cellNode.text().set(str);
					}
					else
					{
						return false;
					}
				}
				else if (dataType == KnowledgeDataType::FLOAT)
				{
					float d;
					if (kt->getData(d, rindex, cindex))
					{
						cellNode.text().set(d);
					}
					else
					{
						return false;
					}
				}
				else if (dataType == KnowledgeDataType::INT)
				{
					int d;
					if (kt->getData(d, rindex, cindex))
					{
						cellNode.text().set(d);
					}
					else
					{
						return false;
					}
				}
				else if (dataType == KnowledgeDataType::STRING)
				{
					std::string d;

					if (kt->getData(d, rindex, cindex))
					{
						cellNode.text().set(d.c_str());
					}
					else
					{
						return false;
					}
				}
				else
				{
					// error occured
					return false;
					break;
				}
			}
		}
		
		// save doc
		return doc.save_file(xmlPath.c_str());
	}

	return false;
}

