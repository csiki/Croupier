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
	size_t numOfBots;
	BotInfo **bots;
	std::vector<Card> cards;

public:
	Table(int numOfBots)
	{
		// we increase numofbots, when a bot sits down
		this->numOfBots = 0;
		this->bots = new BotInfo*[numOfBots];

		// clear bots array
		for (size_t i = 0; i < numOfBots; ++i)
		{
			this->bots[i] = nullptr;
		}
	}

	virtual ~Table() {}

	void addCard(const Card& c); // UNIT done
	int getNumOfCards() const;
	Card rmCard();
	Card getCard(size_t index) const; // UNIT done
	const BotInfo* getBotByIndex(int index) const;
	int getNumOfBots() const;
	int getPot() const;
	void sit(BotInfo* bot);
};

#endif  //_TABLE_H
