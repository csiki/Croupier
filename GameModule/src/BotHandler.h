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

	BotHandler(int playerID, BroadcastStation* broadcastStation, Loggable* loggable, int chips) : 
		BotInfo(playerID, broadcastStation, loggable, chips), Entity(playerID)
	{
		this->stepToken = false;
		this->talkToken = false;
	}
	
	const Card* checkCard(int index);
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
	virtual void rebuyOrLeave() = 0;
	virtual int getBotID() const = 0;
	virtual std::string getName() const = 0;
	virtual BotLanguage getLang() const = 0;
};

#endif  //_BOTHANDLER_H
