#if !defined(_TABLE_H)
#define _TABLE_H

#include "BotInfo.h"
#include "Card.h"
#include "Rulz.h"

class Table
{
private:
	int numOfCards;
	int numOfBots;
	BotInfo *bots;
	Card *cards;
	Rulz *rules;
public:
	void provideBotInfos(int numOfBots, BotInfo* bis);
	void addCard(const Card* const c);
	int getNumOfCards();
	const Card* const rmCard(int cardIndex);
	const Card* const getCard(int cardIndex);
	const BotInfo* const getBotByIndex(int index);
	int getNumOfBots();
	int getPot();
	void sit(const BotInfo* const bot);
	void getUp(const BotInfo* const bot);
};

#endif  //_TABLE_H
