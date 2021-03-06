#if !defined(_BOTINFO_H)
#define _BOTINFO_H

#include <string>
#include <list>
#include "Card.h"
#include "Emotion.h"
#include "BotLanguage.h"
#include "Logger.h"
#include "BroadcastMember.h"
#include "BroadcastStation.h"

/**	Interface for AIs to other AIs.
*/
class BotInfo : public BroadcastMember
{
protected:
	int chips;
	int pot;
	int numOfRaises;
	bool cardsRevealed;
	bool dealer;
	bool inGame;
	bool inRound;
	std::vector<Card> hand;
	Emotion emotion;
public:

	BotInfo(int botID, BroadcastStation* broadcastStation, int chips) :
		Entity(botID), BroadcastMember(botID, broadcastStation)
	{
		this->chips = chips;
		this->pot = 0;
		this->cardsRevealed = false;
		this->dealer = false;
		this->inGame = true;
		this->inRound = false;
		this->emotion = Emotion::HIDDEN;
	}

	int getChips() const;
	int getPot() const;
	Emotion getEmotion() const;
	bool isDealer() const;
	bool isHandRevealed() const;
	bool isInGame() const;
	bool isInRound() const;
	Card lookAtHand(size_t cardIndex) const;

	virtual int getBotID() const = 0;
	virtual std::string getName() const = 0;
	virtual BotLanguage getLang() const = 0;
};

#endif  //_BOTINFO_H
