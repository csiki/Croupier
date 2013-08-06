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

// TODO free shit everywhere !
// TODO "UNIT" tagot mindenhova ahol unit test szükséges - bonyolultabb algóknál, objektum együttmûködõseknél

/**	Directs the game, and the bots; deals cards.
*/
class Croupier : public Logger, public BroadcastMember
{
private:
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
	int* kicksAtRound;
	const Rulz* rules;
	Table* table;
	HandEvaluator handEvaluator;

	bool botComparatorByPot(int botIndex1, int botIndex2);
	int nextActiveBot(int from = -1) const; // default: -1 ~ currentBotIndex
	void burn(Card* c);
	void collectCards();
	void betRound();
	void dealing();
	void preflop();
	void flop();
	void turn();
	void river();
	void showdown();
	void handOutPot(int winnerIndex);
	void handOutPot(int numOfWinners, int* winnersIndex);
	void refreshBlinds();
	void determineWinners(int& numOfWinners, int** winnersIndex);
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
		this->kicksAtRound = new int[numOfBots];
	}

	virtual ~Croupier()
	{
		delete [] this->bots;
	}

	void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data);
	void letsPoker();
	int getKickAtRound(int botIndex) const;
	int getBotIDByIndex(int botIndex) const;
	void provideBotHandler(int index, BotHandler* bh);
};

#endif  //_CROUPIER_H
