#if !defined(_PROCESS_H)
#define _PROCESS_H

// TODO neki még lesznek adattagjai, amin keresztül eléri a futó processt

/**	Handles other than its own processes.
*/
class Process
{
public:
	void start();
	std::string readOutput();
	void writeInput(const std::string msg);
	void exit();
};

#endif  //_PROCESS_H
