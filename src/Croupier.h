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

class Croupier : public Logger, public BroadcastMember, public Logger
{
private:
	Card** burnt;
	int numberOfBurntCards;
	Rulz* const rules;
	int round;
	int currentBotIndex;
	int lastBotRaisedIndex;
	int nextBlindShiftAt;
	int* kicksAtRound;
	int numOfBots;
	void burn(const Card* const c);
	void collectCards();
	void betRound();
	void fillDeck(Deck& deck);
	void dealing();
	int nextActiveBot(int from = currentBotIndex);
	void preflop();
	void flop();
	void turn();
	void river();
	void showdown();
	void handOutPot(int winnerIndex);
	void handOutPot(int numOfWinners, const int* const winnersIndex);
	void refreshBlinds();
	void determineWinners(int numOfWinners, int* winnersIndex);
	Deck *deck;
	Table *table;
	BotHandler *bots;
	BroadcastStation *broadcast;
	HandEvaluator *handEvaluator;
public:
	void letsPoker();
	int getKickAtRound(int botIndex);
	int getBotIDByIndex(int botIndex);
	void provideBotHandlers(int numOfBots, BotHandler* bhs);
	void provideTable(Table* const table);
	void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* const data);
	void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* const data);
	void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* const data);
};

#endif  //_CROUPIER_H
