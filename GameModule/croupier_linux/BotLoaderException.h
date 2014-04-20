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

    ~BotLoaderException() noexcept {}

	std::string whatError() const
	{
		return this->error;
	}

	const char* what() const noexcept
	{
	    return "Error during bot loading!";
	}
};

#endif // _BOTLOADEREXCEPTION_H
