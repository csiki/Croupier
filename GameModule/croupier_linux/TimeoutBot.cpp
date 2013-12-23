#if !defined(_TIMEOUTBOT_H)
#define _TIMEOUTBOT_H

#include <string>
#include <vector>
#include <list>
#include <map>
#include <cstdlib>
#include <time.h>
#include "Bot.h"
#include "BotCommunicator.h"
#include "BotLanguage.h"
#include "BotManager.h"
#include "BotInfo.h"
#include "Card.h"
#include "BettingSystem.h"
#include "Comment.h"
#include "Emotion.h"
#include "HandRank.h"

class TimeoutBot : public Bot
{
public:
	TimeoutBot(BotCommunicator* communicator, int id, std::string name, BotLanguage lang)
		: Bot(communicator, id, name, lang)
	{
		time_t seconds;
		time(&seconds);
		srand((unsigned int) seconds);
	}

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
		if(rand() % 2)
			while(true);
	}

	void turn()
	{
	}
};


/// class factory definitions
extern "C" Bot* create(BotCommunicator* communicator, int id, std::string name, BotLanguage lang)
{
    return new TimeoutBot(communicator, id, name, lang);
}

extern "C" void destroy(Bot* bot)
{
    delete bot;
}

#endif // _TIMEOUTBOT_H
