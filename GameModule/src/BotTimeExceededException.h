#if !defined(_BOTTIMEEXCEEDEDEXCEPTION_H)
#define _BOTTIMEEXCEEDEDEXCEPTION_H

/** Exception for identifying bot calculation overtime.
*/
class BotTimeExceededException : public exception
{
private:
	string inMethod;

public:
	BotTimeExceededException(string inMethod)
		: exception("Bot calculation time exceeded!"), inMethod(inMethod) {}

	string whatMethod() const
	{
		return this->inMethod;
	}
};

#endif