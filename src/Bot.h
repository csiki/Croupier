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
	void allined(int botID, int amount);
	void blindsRaised(int newSmallBlind, int newBigBlind);
	void called(int botID, int amount);
	void checked(int botID);
	void flop();
	void folded(int botID);
	void gameWinner(int botID);
	int getID();
	int getLang();
	const char* const getName();
	void leftGame(int botID);
	void listen(int botID, Comment comment);
	void preflop();
	void quit();
	void raised(int botID, int amount);
	void rebuyDeadlineReached();
	void rebuyOrLeave();
	void river();
	void roundEnded();
	void roundStarted(int round);
	void roundWinners(int numOfWinners, const int* const winners);
	void showdown();
	void step();
	void turn();
}

#endif  //_BOT_H