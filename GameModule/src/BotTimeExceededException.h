#if !defined(_BOTTIMEEXCEEDEDEXCEPTION_H)
#define _BOTTIMEEXCEEDEDEXCEPTION_H

/** Exception for identifying bot calculation overtime.
*/
class BotTimeExceededException : public std::exception
{
private:
	std::string inMethod;

public:
	BotTimeExceededException(std::string inMethod)
		: std::exception("Bot calculation time exceeded!"), inMethod(inMethod) {}

	std::string whatMethod() const
	{
		return this->inMethod;
	}
};

#endif