#if !defined(_TABLE_H)
#define _TABLE_H

#include "BotInfo.h"
#include "Card.h"
#include "Rulz.h"

/**	Contains those (interfaces of) entities and informations that are available for AIs.
*/
class Table
{
private:
	int numOfBots;
	BotInfo ** bots;
	int numOfCards;
	Card **cards;
public:
	// TODO konstruktorba megkapja numOfBots-ot, így le tudja foglalni a helyet nekik, mire leülnek
	Table(int numOfBots);
	void addCard(Card* c);
	int getNumOfCards() const;
	Card* rmCard(int cardIndex);
	const Card* getCard(int index) const;
	const BotInfo* getBotByIndex(int index) const;
	int getNumOfBots() const;
	int getPot() const;
	void sit(BotInfo* bot);
	void getUp(BotInfo* bot);
};

#endif  //_TABLE_H
