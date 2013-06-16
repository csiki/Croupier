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
private:
	Deck* deck;
	Card** burnt;
	int numberOfBurntCards;
	int round;
	int currentBotIndex;
	int lastBotRaisedIndex;
	int nextBlindShiftAt;
	int numOfBots;
	BotHandler** bots;
	int* kicksAtRound;
	const Rulz* rules;
	Table* table; // direkt nem konstans
	BroadcastStation* broadcast;
	HandEvaluator* handEvaluator;

	int nextActiveBot(int from = -1) const; // parameter from default: currentBotIndex
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
	void handOutPot(int numOfWinners, const int* winnersIndex);
	void refreshBlinds();
	void determineWinners(int numOfWinners, int* winnersIndex);
public:
	void letsPoker();
	int getKickAtRound(int botIndex) const;
	int getBotIDByIndex(int botIndex) const;
	void provideBotHandlers(int numOfBots, BotHandler** bhs);
	void provideTable(const Table* table);
	void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data);
};

#endif  //_CROUPIER_H
