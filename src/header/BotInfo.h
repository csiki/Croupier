#if !defined(_BOTINFO_H)
#define _BOTINFO_H

#include "Card.h"

class BotInfo
{
private:
	Card *hand;
protected:
	int chips;
	int pot;
	bool dealer;
	bool cardsRevealed;
	bool inGame;
	bool inRound;
public:
	int getChips();
	virtual int getID() = 0;
	virtual const char* const getName() = 0;
	virtual int getLang() = 0;
	int getPot();
	bool isDealer();
	bool isHandRevealed();
	bool isInGame();
	bool isInRound();
	const Card* const lookAtHand(int cardIndex);
};

#endif  //_BOTINFO_H
