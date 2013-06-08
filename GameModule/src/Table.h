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
	BotInfo **bots;
	int numOfCards;
	Card **cards;
public:
	// konstruktorba megkapja numOfBots-ot, így le tudja foglalni a helyet nekik, mire leülnek
	void addCard(Card* c);
	int getNumOfCards() const;
	Card* rmCard(int cardIndex);
	const Card* getCard(int cardIndex) const;
	const BotInfo* getBotByIndex(int index) const;
	int getNumOfBots() const;
	int getPot() const;
	void sit(const BotInfo* bot);
	void getUp(const BotInfo* bot);
};

#endif  //_TABLE_H
