#if !defined(_PROCESS_H)
#define _PROCESS_H

// TODO neki m�g lesznek adattagjai, amin kereszt�l el�ri a fut� processt

/**	Handles other than its own processes.
*/
class Process
{
public:
	Process(std::string programpath)
	{
	}

	virtual ~Process()
	{
	}

	void start(std::list<std::string> arguments); // UNIT
	std::string readOutput(); // UNIT
	void writeInput(const std::string msg); // UNIT
	void exit(); // UNIT
};

#endif  //_PROCESS_H
