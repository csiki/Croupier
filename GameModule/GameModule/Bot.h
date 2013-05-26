#if !defined(_BOT_H)
#define _BOT_H

#include "Comment.h"
#include "BotCommunicator.h"

class Bot
{
private:
	int id;
	const char* const name;
	const int lang;
public:
	int getID();
	int getLang();
	const char* getName();
	
	virtual void allined(int botID, int amount) = 0;
	virtual void blindsRaised(int newSmallBlind, int newBigBlind) = 0;
	virtual void called(int botID, int amount) = 0;
	virtual void checked(int botID) = 0;
	virtual void flop() = 0;
	virtual void folded(int botID) = 0;
	virtual void gameWinner(int botID) = 0;
	virtual void leftGame(int botID) = 0;
	virtual void listen(int botID, Comment comment) = 0;
	virtual void preflop() = 0;
	virtual void quit() = 0;
	virtual void raised(int botID, int amount) = 0;
	virtual void rebuyDeadlineReached() = 0;
	virtual void rebuyOrLeave() = 0;
	virtual void river() = 0;
	virtual void roundEnded() = 0;
	virtual void roundStarted(int round) = 0;
	virtual void roundWinners(int numOfWinners, const int* winners) = 0;
	virtual void showdown() = 0;
	virtual void step() = 0;
	virtual void turn() = 0;
};

#endif  //_BOT_H