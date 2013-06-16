#if !defined(_BOTINFO_H)
#define _BOTINFO_H

#include "Card.h"
#include "Emotion.h"

/**	Interface for AIs to other AIs.
*/
class BotInfo
{
protected:
	int numOfCardsInHand;
	int chips;
	int pot;
	bool cardsRevealed;
	bool dealer;
	bool inGame;
	bool inRound;
	Card **hand;
	Emotion emotion;
public:
	int getChips() const;
	int getPot() const;
	Emotion getEmotion() const;
	bool isDealer() const;
	bool isHandRevealed() const;
	bool isInGame() const;
	bool isInRound() const;
	Card lookAtHand(int cardIndex) const;

	virtual int getID() const = 0;
	virtual std::string getName() const = 0;
	virtual int getLang() const = 0;
};

#endif  //_BOTINFO_H
