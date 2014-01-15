#if !defined(_ALIVEBOT_H)
#define _ALIVEBOT_H

#include "Bot.h"
#include "BotLanguage.h"
#include "BettingSystem.h"
#include "KnowledgeDataType.h"

// TODO talk?

class AliveBot : public Bot
{
    const int tabla_id;

public:
	AliveBot(BotCommunicator* communicator, int id, std::string name, BotLanguage lang)
		: Bot(communicator, id, name, lang), tabla_id(0)
    {
        if (communicator->isBotKnowledgeUseAllowed()
            && !communicator->isTableLoaded(tabla_id))
        {
            // csak akkor hozzuk létre ha még nem létezik.
            // feljegyezzük botonként, hogy melyik id-ju bot
            // mennyire aggresszív (persze azok közül akikkel
            // eddig sikerült játszani).
            std::list<KnowledgeDataType> oszlopAdatTipusok;
            oszlopAdatTipusok.push_back(KnowledgeDataType::INT); // bot id-je
            oszlopAdatTipusok.push_back(KnowledgeDataType::FLOAT); // aggr értéke

            communicator->createKnowledgeTable(2, oszlopAdatTipusok);
        }
    }

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
		if (communicator->isBotKnowledgeUseAllowed())
        {
            novelAggr(botID, -0.1); // egy kicsivel növelünk
        }
	}

	void checked(int botID)
	{
		if (communicator->isBotKnowledgeUseAllowed())
        {
            novelAggr(botID, -0.1); // egy kicsivel csökkentünk
        }
	}

	void flop()
	{
		// nothing to do here
	}

	void folded(int botID)
	{
		if (communicator->isBotKnowledgeUseAllowed())
        {
            novelAggr(botID, -0.2); // csökkentjük az aggr pontját
        }
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
        if (amount > 2 * communicator->getBigBlindAtRound() // csak ha serious emelés
            && communicator->isBotKnowledgeUseAllowed())
        {
            novelAggr(botID, 0.2); // 0.2-vel növeljük ha emel
        }
	}

	void novelAggr(int botID, float mennyivel)
	{
        int bid, sor = 0;
        float val = 0.0;
        bool megvan = false; // apróságok
        while (communicator->getKnowledgeTableData(bid, tabla_id, sor, 0))
        {
            if (bid == botID)
            {
                megvan = true;
                communicator->getKnowledgeTableData(val, tabla_id, sor, 1);
                break;
            }

            ++sor;
        }
        if (!megvan)
        {
            sor = communicator->addKnowledgeTableRow(tabla_id);
            communicator->setKnowledgeTableData(botID, tabla_id, sor, 0);
        }

        val += mennyivel; // növeljük aggr pontot (csökkentjük ha mennyivel negatív)
        communicator->setKnowledgeTableData(val, tabla_id, sor, 1); // visszamentjük az értéket
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
		std::cout << "Enter the amount of credits " << this->getName() << " want to get back with! Zero means no rebuy !" << std::endl;

		do {
			std::cout << "Rebuy credits: ";
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
		std::string move = "";
		int param;
		bool success;
		std::cout << this->getName() << " moves now !" << std::endl;

		do {
			success = false;
			do {
				std::cout << "Type check, call, raise or fold: ";
				std::cin >> move;
				if (move == "raise")
				{
					std::cout << "How much: ";
					std::cin >> param;
				}
				else
				{
					param = 1;
				}
			} while (move != "check" && move != "call" && move != "raise" && move != "fold" && move != "allin" && param > 0);

			if (move == "check")
			{
				success = this->communicator->canCheck();
			}
			else if (move == "call")
			{
				success = this->communicator->canCall();
			}
			else if (move == "raise")
			{
				success = this->communicator->canRaise(param);
			}
			else if (move == "fold")
			{
				success = this->communicator->canFold();
			}
			else if (move == "allin")
			{
				success = this->communicator->canAllin();
			}
			else
			{
				success = false;
			}
		} while (!success);

		// take the move
		if (move == "check")
		{
			success = this->communicator->check();
		}
		else if (move == "call")
		{
			success = this->communicator->call();
		}
		else if (move == "raise")
		{
			success = this->communicator->raise(param);
		}
		else if (move == "fold")
		{
			success = this->communicator->fold();
		}
		else if (move == "allin")
		{
			success = this->communicator->allin();
		}
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
