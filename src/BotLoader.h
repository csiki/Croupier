#if !defined(_BOTLOADER_H)
#define _BOTLOADER_H


class BotLoader
{
public:
	virtual Bot* loadBot(const char* const botFilePath) = 0;
protected:
	const int botLang;
};

#endif  //_BOTLOADER_H
