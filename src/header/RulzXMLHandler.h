#if !defined(_RULZXMLHANDLER_H)
#define _RULZXMLHANDLER_H


class RulzXMLHandler
{
public:
	static Rulz* loadXML(const char* const xmlPath);
	static bool saveXML(Rulz* log, const char* const xmlPath);
};

#endif  //_RULZXMLHANDLER_H
