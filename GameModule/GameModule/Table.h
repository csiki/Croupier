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
	void addCard(Card* c);
	int getNumOfCards();
	Card* rmCard(int cardIndex);
	Card* getCard(int cardIndex);
	BotInfo* getBotByIndex(int index);
	int getNumOfBots();
	int getPot();
	void sit(BotInfo* bot);
	void getUp(BotInfo* bot);
};

#endif  //_TABLE_H
