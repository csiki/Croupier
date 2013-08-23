#if !defined(_TESTBOTSHY_H)
#define _TESTBOTSHY_H

#include "Bot.h"
#include "BotLanguage.h"
#include "BettingSystem.h"

class TestBotShy : public Bot
{
public:
	TestBotShy(int id, string name, BotLanguage lang) : Bot(id, name, lang) {}

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
		cout << "step" << endl;
	}

	void turn()
	{
	}
};

#endif // _TESTBOTSHY_H