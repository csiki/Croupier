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

class Croupier : public Logger, public BroadcastMember
{
private:
	Card** burnt;
	int numberOfBurntCards;
	const Rulz* rules;
	int round;
	int currentBotIndex;
	int lastBotRaisedIndex;
	int nextBlindShiftAt;
	int* kicksAtRound;
	int numOfBots;
	Deck* deck;
	const Table* table;
	BotHandler** bots;
	BroadcastStation* broadcast;
	HandEvaluator* handEvaluator;

	void burn(Card* c);
	void collectCards();
	void betRound();
	void fillDeck(Deck& deck);
	void dealing();
	int nextActiveBot(int from = -1); // from default: currentBotIndex
	void preflop();
	void flop();
	void turn();
	void river();
	void showdown();
	void handOutPot(int winnerIndex);
	void handOutPot(int numOfWinners, const int* winnersIndex);
	void refreshBlinds();
	void determineWinners(int numOfWinners, int* winnersIndex);
public:
	void letsPoker();
	int getKickAtRound(int botIndex);
	int getBotIDByIndex(int botIndex);
	void provideBotHandlers(int numOfBots, BotHandler** bhs);
	void provideTable(const Table* table);
	void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data);
};

#endif  //_CROUPIER_H
