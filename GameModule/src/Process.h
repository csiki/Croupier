#if !defined(_PROCESS_H)
#define _PROCESS_H

// TODO neki m�g lesznek adattagjai, amin kereszt�l el�ri a fut� processt

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
