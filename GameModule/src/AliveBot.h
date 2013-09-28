#if !defined(_ALIVEBOT_H)
#define _ALIVEBOT_H

#include "Bot.h"
#include "BotLanguage.h"
#include "BettingSystem.h"

class AliveBot : public Bot
{
public:
	AliveBot(BotCommunicator* communicator, int id, std::string name, BotLanguage lang)
		: Bot(communicator, id, name, lang) {}

	void allined(int botID, int amount)
	{
		// nothing to do here
	}

	void blindsRaised(int newSmallBlind, int newBigBlind)
	{
		// nothing to do here
	}

	void called(int botID, int amount)
	{
		// nothing to do here
	}

	void checked(int botID)
	{
		// nothing to do here
	}

	void flop()
	{
		// nothing to do here
	}

	void folded(int botID)
	{
		// nothing to do here
	}

	void gameWinner(int botID)
	{
		// nothing to do here
	}

	void leftGame(int botID)
	{
		// nothing to do here
	}

	void listen(int botID, Comment comment)
	{
		// nothing to do here
	}

	void preflop()
	{
		// nothing to do here
	}

	void leave()
	{
		// nothing to do here
	}

	void raised(int botID, int amount)
	{
		// nothing to do here
	}

	void rebuyOccurred(int botID, int amount)
	{
		// nothing to do here
	}

	void rebuyDeadlineReached()
	{
		// nothing to do here
	}

	void rebuyOrLeave()
	{
		int rebuyAmount;
		std::cout << "Rebuy or leave to human named " << this->getName() << " !" << std::endl;
		std::cout << "Enter the amount of credits " << this->getName() << " want to get back with! Zero means no rebuy!" << std::endl;
		std::cout << "Rebuy credits: ";
		
		do {
			std::cin >> rebuyAmount;
		} while (!this->areYouSure());

		if (rebuyAmount > 0)
		{
			this->communicator->rebuy(rebuyAmount);
		}
	}

	void river()
	{
		// nothing to do here
	}

	void roundEnded()
	{
		// nothing to do here
	}

	void roundStarted(int round)
	{
		// nothing to do here
	}

	void roundWinners(int numOfWinners, const int* winners)
	{
		// nothing to do here
	}

	void showdown()
	{
		// nothing to do here
	}

	void step()
	{
		std::string move;
		int param;
		std::cout << this->getName() << " moves now !" << std::endl;
		std::cout << "Type check, call, raise or fold: ";
		
		do {
			std::cin >> move;
		} while(move != "check" && move != "call" && move != "raise" && move != "fold" && move != "allin");

		//if (raise --> param bevétele) // TODO

		do {
			//std::cin >> rebuyAmount;
		} while (!this->areYouSure());
	}

	void turn()
	{
		// nothing to do here
	}

	// own
	bool areYouSure()
	{
		char answer = 'n';
		std::cout << "Are you sure (press y/n) : ";
		std::cin >> answer;

		if (answer == 'y')
		{
			return true;
		}
		
		return false;
	}
};

#endif // _ALIVEBOT_H