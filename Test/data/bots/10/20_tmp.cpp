#include <string>
#include <vector>
#include <list>
#include <map>
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

class Jonas2 : public Bot
{

  	int numCalls = 0;
public:
	Jonas2(BotCommunicator* communicator, int id, std::string name, BotLanguage lang)
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
    	numCalls = 0; 
	}

	void roundWinners(int numOfWinners, const int* winners)
	{
	}

	void showdown()
	{
	}

	void step()
	{
		if (communicator->getHandRank() >= HandRank::Pair)
		{
			/* ha már pár vagy annál jobb lapja van (az asztalival együtt),
			 emel ha tud
			 meghív ha tud
			 csekkel ha tud
			 minden más esetben allin-ol */
          	if (communicator->canCall() && numCalls < 3)
            {
              	numCalls++;
				communicator->call();
            }
			else if (communicator->canCheck())
				communicator->check();
            else
				communicator->fold();
              
		}
		else
		{
			/* ha párja sincs, akkor jámbor taktikára vált
			 csekkel ha tud
			 meghív, ha a meghívandó összeg, amit még
			 be kell pakolnia, kevesebb mint a zsetonjainak fele
			 egyébként bedobja a lapjait */
			if (communicator->canCheck())
				communicator->check();
			else if (communicator->getCallAmount() < communicator->getChips() / 2)
				communicator->call();
			else
				communicator->fold();
		}
	}

	void turn()
	{
	}
};

extern "C" Bot* create(BotCommunicator* communicator, int id, std::string name, BotLanguage lang){return new Jonas2(communicator, id, name, lang);}extern "C" void destroy(Bot* bot){delete bot;}