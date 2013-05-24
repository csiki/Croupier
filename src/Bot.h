#if !defined(_BOTCOMMUNICATOR_H)
#define _BOTCOMMUNICATOR_H

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
}