#if !defined(_BOTCOMMUNICATIONTRANSLATOR_H)
#define _BOTCOMMUNICATIONTRANSLATOR_H

#include "Process.h"
#include "Comment.h"
#include "Card.h"
#include "Bot.h"

/**	Base class for translators.
*/
class BotCommunicationTranslator : public Bot
{
private:
	Process* process;
protected:
	virtual void interpret(const string) = 0;
	virtual void send(bool msg) = 0;
	virtual void send(Card& msg) = 0;
	virtual void send(Comment msg) = 0;
	virtual void send(int msg) = 0;
	virtual void send(int msgSize, const int* msg) = 0;
	virtual void send(string msg) = 0;
public:
	
	BotCommunicationTranslator(int id, string name, int langID, Process* process) : Bot(id, name, langID)
	{
		this->process = process;
	}

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
	virtual void rebuyOccurred(int botID, int amount);
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

#endif //_BOTCOMMUNICATIONTRANSLATOR_H