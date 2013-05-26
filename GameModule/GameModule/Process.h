#if !defined(_PROCESS_H)
#define _PROCESS_H


class Process
{
public:
	void start();
	char* readOutput();
	void writeInput(const char* msg);
	void exit();
};

#endif  //_PROCESS_H
