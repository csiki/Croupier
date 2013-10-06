#if !defined(_TESTBOTGRESSIVE_H)
#define _TESTBOTGRESSIVE_H

#include "Bot.h"
#include "BotLanguage.h"
#include "BettingSystem.h"

class TestBotAggressive : public Bot
{
public:
	TestBotAggressive(BotCommunicator* communicator, int id, std::string name, BotLanguage lang)
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
		std::cout << "step aggressivebot" << std::endl;
		while(1) {} // TEST
		if (this->communicator->getCallAmount() == 0)
		{
			if (!this->communicator->raise(this->communicator->getBigBlindAtRound()))
			{
				this->communicator->allin();
			}
		}
		else if (this->communicator->getCallAmount() > this->communicator->getBigBlindAtRound())
		{
			if (!this->communicator->call())
			{
				this->communicator->allin();
			}
		}
		else
		{
			if (!this->communicator->raise(this->communicator->getBigBlindAtRound()))
			{
				this->communicator->allin();
			}
		}
	}

	void turn()
	{
	}
};

#endif // _TESTBOTGRESSIVE_H