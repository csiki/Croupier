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
	Card* cards[5];

public:
	Table(int numOfBots)
	{
		// we increase numofbots, when a bot sits down
		this->numOfBots = 0;
		this->numOfCards = 0;
		this->bots = new BotInfo*[numOfBots];

		// clear bots array
		for (int i = 0; i < numOfBots; ++i)
		{
			this->bots[i] = 0;
		}

		// clear cards array
		for (int i = 0; i < 5; ++i)
		{
			this->cards[i] = nullptr;
		}
	}

	virtual ~Table() {}

	void addCard(Card* c);
	int getNumOfCards() const;
	Card* rmCard();
	const Card* getCard(int index) const;
	const BotInfo* getBotByIndex(int index) const;
	int getNumOfBots() const;
	int getPot() const;
	void sit(BotInfo* bot);
};

#endif  //_TABLE_H
