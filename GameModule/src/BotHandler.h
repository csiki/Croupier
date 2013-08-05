#if !defined(_BOTHANDLER_H)
#define _BOTHANDLER_H

#include "BotInfo.h"

/**	Interface for the Croupier to direct AIs.
*/
class BotHandler : virtual public BotInfo
{
protected:
	bool stepToken;
	bool talkToken;
public:

	BotHandler(int chips) : BotInfo(chips)
	{
		this->stepToken = false;
		this->talkToken = false;
	}

	void addDealerButton();
	void rmDealerButton();
	void forceBlind(int blind);
	void receiveCard(Card* c);
	void receiveChips(int chipsAmount);
	void revealCards();
	Card* takeHand(int cardIndex);
	int takePot(int amount = -1); // -1 default means all the pot
	virtual void step() = 0;
	virtual void leave() = 0;
	virtual bool rebuyOrLeave() = 0;
	virtual int getID() const = 0;
	virtual std::string getName() const = 0;
	virtual int getLang() const = 0;
};

#endif  //_BOTHANDLER_H
