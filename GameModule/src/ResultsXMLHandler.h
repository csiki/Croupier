#if !defined(_RESULTSXMLHANDLER_H)
#define _RESULTSXMLHANDLER_H

#include "pugixml.hpp"
#include "Results.h"

/**	Loads and saves bot results (botid-credit and botid-botfalloutround) in xml format.
*/
class ResultsXMLHandler
{
public:
	static Results* loadXML(std::string xmlPath); // UNIT done
	static bool saveXML(Results* results, std::string xmlPath); // UNIT done
};

#endif  //_RESULTSXMLHANDLER_H
