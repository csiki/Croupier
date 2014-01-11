#if !defined(_TIMERBOTPROXY_H)
#define _TIMERBOTPROXY_H

#include "Bot.h"

/** Provides a proxy for botmanager to an AI in order to check if the AI runs longer than it is allowed.
*/
class TimerBotProxy : public Bot
{
private:
	Bot* const forwardTo;
	const int allowedCalcTime; // in millisec

protected:
	bool isInTime(std::atomic_bool& exited);
	void handleTimeout(std::string inMethod);

public:
	TimerBotProxy(Bot* bot, int allowedCalcTime)
		: Bot(nullptr, bot->getID(), bot->getName(), bot->getLang()), forwardTo(bot), allowedCalcTime(allowedCalcTime) {}
	virtual ~TimerBotProxy() {}

	// inherited from bot
	void allined(int botID, int amount);
	void blindsRaised(int newSmallBlind, int newBigBlind);
	void called(int botID, int amount);
	void checked(int botID);
	void flop();
	void folded(int botID);
	void gameWinner(int botID);
	void leftGame(int botID);
	void listen(int botID, Comment comment);
	void preflop();
	void leave();
	void raised(int botID, int amount);
	void rebuyOccurred(int botID, int amount);
	void rebuyDeadlineReached();
	void rebuyOrLeave();
	void river();
	void roundEnded();
	void roundStarted(int round);
	void roundWinners(int numOfWinners, const int* winners);
	void showdown();
	void step();
	void turn();
};

#endif  //_TIMERBOTPROXY_H
