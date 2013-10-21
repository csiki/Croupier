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
		: std::exception("Error during bot loading!"), error(error) {}

	std::string whatError() const
	{
		return this->error;
	}
};

#endif // _BOTLOADEREXCEPTION_H