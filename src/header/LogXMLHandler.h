#if !defined(_LOGXMLHANDLER_H)
#define _LOGXMLHANDLER_H


class LogXMLHandler
{
public:
	static Log* loadXML(const char* const xmlPath);
	static bool saveXML(Log* log, const char* const xmlPath);
};

#endif  //_LOGXMLHANDLER_H
