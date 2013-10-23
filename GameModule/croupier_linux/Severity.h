#if !defined(_SEVERITY_H)
#define _SEVERITY_H

/**	Enum of event severities.
*/
enum Severity
{
	FATAL = 0,			// Problem with the system itself.
	ERROR = 1,			// Bot violates rules like allocating too much memory or taking too much time. These actions typically can NOT be ignored.
	WARNING = 2,		// Bot wants to take action that is not permitted (like checking when calling/raising is possible or talking/expressing emotions/using bot knowledge tables when not possible) or not take step when possible. These actions are typically ignored.
	NOTIFICATION = 3,	// Croupier takes action (flop, raising blinds etc.).
	INFORMATION = 4,	// Bot takes action, talks or changes emotion.
	VERBOSE = 5,		// Bot coommunicates with BotManager.
	DEBUG = 6			// Only for debug purposes.
};

#endif  //_SEVERITY_H
