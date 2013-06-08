#if !defined(_BOTINFO_H)
#define _BOTINFO_H

#include "Card.h"
#include "Emotion.h"


/**	Interface for AIs to other AIs.
*/
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

	virtual int getID() = 0;
	virtual std::string getName() = 0;
	virtual int getLang() = 0;
};

#endif  //_BOTINFO_H
