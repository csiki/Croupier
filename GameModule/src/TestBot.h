#if !defined(_TESTBOT_H)
#define _TESTBOT_H

#include "Bot.h"
#include "BotLanguage.h"
#include "BettingSystem.h"

class TestBot : public Bot
{
public:
	TestBot(BotCommunicator* communicator, int id, string name, BotLanguage lang)
		: Bot(communicator, id, name, lang) {}

	void allined(int botID, int amount)
	{

	}

	void blindsRaised(int newSmallBlind, int newBigBlind)
	{

	}

	void called(int botID, int amount)
	{

	}

	void checked(int botID)
	{

	}

	void flop()
	{

	}

	void folded(int botID)
	{

	}

	void gameWinner(int botID)
	{

	}

	void leftGame(int botID)
	{

	}

	void listen(int botID, Comment comment)
	{

	}

	void preflop()
	{

	}

	void leave()
	{

	}

	void raised(int botID, int amount)
	{

	}

	void rebuyOccurred(int botID, int amount)
	{
	}

	void rebuyDeadlineReached()
	{

	}

	void rebuyOrLeave()
	{
	}

	void river()
	{
	}

	void roundEnded()
	{
	}

	void roundStarted(int round)
	{
	}

	void roundWinners(int numOfWinners, const int* winners)
	{
	}

	void showdown()
	{
	}

	void step()
	{
		cout << "step testbot" << endl;

		if (this->communicator->getCallAmount() < this->communicator->getChips() && this->communicator->getCallAmount() > 0)
		{
			this->communicator->call();
		}
		else if (this->communicator->getCallAmount() == 0)
		{
			this->communicator->check();
		}
		else
		{
			this->communicator->fold();
		}
	}

	void turn()
	{
	}
};

#endif // _TESTBOT_H