#if !defined(_BOTLOADEREXCEPTION_H)
#define _BOTLOADEREXCEPTION_H

/** Exception for identifying bot loader faults.
*/
class BotLoaderException : public std::exception
{
private:
	std::string error;

public:
	BotLoaderException(std::string error)
		: std::exception(), error(error) {}

	std::string whatError() const
	{
		return this->error;
	}

	const char* what() const _GLIBCXX_USE_NOEXCEPT
	{
	    return "Error during bot loading!";
	}
};

#endif // _BOTLOADEREXCEPTION_H
