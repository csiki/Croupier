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
		: std::exception(), inMethod(inMethod) {}

    ~BotTimeExceededException() noexcept {}

	std::string whatMethod() const
	{
		return this->inMethod;
	}

	const char* what() const noexcept
	{
	    return "Bot calculation time exceeded!";
	}
};

#endif
