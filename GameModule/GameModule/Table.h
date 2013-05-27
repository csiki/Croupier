#if !defined(_TABLE_H)
#define _TABLE_H

#include "BotInfo.h"
#include "Card.h"
#include "Rulz.h"

class Table
{
private:
	int numOfBots;
	BotInfo **bots;
	int numOfCards;
	Card **cards;
	Rulz *rules;
public:
	void provideBotInfos(int numOfBots, BotInfo** bis);
	void addCard(Card* c);
	int getNumOfCards();
	Card* rmCard(int cardIndex);
	const Card* getCard(int cardIndex);
	const BotInfo* getBotByIndex(int index);
	int getNumOfBots();
	int getPot();
	void sit(const BotInfo* bot);
	void getUp(const BotInfo* bot);
};

#endif  //_TABLE_H
