#if !defined(_CROUPIER_H)
#define _CROUPIER_H

#include "Logger.h"
#include "BroadcastMember.h"
#include "Deck.h"
#include "Table.h"
#include "BotHandler.h"
#include "BroadcastStation.h"
#include "HandEvaluator.h"
#include "BroadcastMessage.h"

/**	Directs the game, and the bots; deals cards.
*/
class Croupier : public Logger, public BroadcastMember
{
public: // TODO private
	Deck deck;
	Card* burnt[3];
	int numberOfBurntCards;
	int round;
	int currentBotIndex;
	int currentDealerIndex;
	int lastBotRaisedIndex;
	int currentBlindIndex;
	int nextBlindShiftAtIndex;
	int numOfBots;
	BotHandler** bots;
	const Rulz* rules;
	Table* table;
public: // TODO private
	bool botComparatorByPot(int botIndex1, int botIndex2); // UNIT done
	int nextActiveBot(int from = -1) const; // default: -1 ~ currentBotIndex // UNIT done
	void burn(Card* c); // UNIT done 
	void collectCards(); // UNIT done 
	void betRound(); // UNIT done 
	void dealing(); // UNIT done 
	void preflop(); // UNIT done 
	void flop(); // UNIT done 
	void turn(); // UNIT done 
	void river(); // UNIT done 
	void showdown(); // UNIT done 
	void handOutPot(int winnerIndex); // UNIT done 
	void handOutPot(int numOfWinners, int* winnersIndex); // UNIT done for 2 playas only 
	void refreshBlinds(); // UNIT done 
	void determineWinners(int& numOfWinners, int** winnersIndex); // UNIT done for 2 players only 
	void kickBot(int botID); 
	int findBotIndexByID(int botID) const;
	bool canStartNewRound() const;
	bool canRoundGoOn() const;
	int numOfBotsInRound() const;

public:
	/** Method to create Croupier, setting its attibrutes to default.
	*/
	Croupier(int numOfBots, BroadcastStation* broadcastStation, Loggable* loggable, const Rulz* rules, Table* table) :
		BroadcastMember(0, broadcastStation), Logger(0, loggable), Entity(0)
	{
		this->rules = rules;
		this->table = table;
		this->numberOfBurntCards = 0;
		this->burnt[0] = nullptr;
		this->burnt[1] = nullptr;
		this->burnt[2] = nullptr;
		this->round = 0;
		this->currentBotIndex = 0;
		this->currentBlindIndex = 0;
		this->currentDealerIndex = 0;
		this->lastBotRaisedIndex = 0;
		this->currentBlindIndex = 0;
		this->nextBlindShiftAtIndex = 0;
		this->numOfBots = numOfBots;
		this->bots = new BotHandler*[numOfBots];

		// subscribe to BroadcastStation
		this->subscribe();
	}

	virtual ~Croupier()
	{
		delete [] this->bots;
	}

	void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data);
	void letsPoker();
	void provideBotHandler(int index, BotHandler* bh);
};

#endif  //_CROUPIER_H
