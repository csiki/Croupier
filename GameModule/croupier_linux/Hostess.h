#if !defined(_HOSTESS_H)
#define _HOSTESS_H

#include "BroadcastMember.h"
#include "Table.h"
#include "Rulz.h"
#include "BroadcastMessage.h"
#include "BotInfo.h"
#include "HandRank.h"
#include "HandEvaluator.h"

/**	Helper for AIs.
*/
class Hostess : public BroadcastMember
{
private:
	const Table* table;
	const Rulz* rules;
	std::map<int, const BotInfo*> botsByID;
	bool* botsInRound; // define at constructor
	bool* botsInGame; // define at constructor
	size_t numOfBots;
	int round;
	int callAmount;
	int minRaise;
	int numberOfRaisesSoFar;
	int nextBlindShiftDeadlineIndex;

	void handleRaise(int raiseAmount); // UNIT done

public:
	/** Create a Hostess instance with id -1.
	*/
	Hostess(int numOfBots, const Table* table, const Rulz* rules, BroadcastStation* broadcastStation) :
		BroadcastMember(-1, broadcastStation), Entity(-1)
	{
		this->table = table;
		this->rules = rules;
		this->numOfBots = numOfBots;

		this->botsInGame = new bool[numOfBots];
		this->botsInRound = new bool[numOfBots];
		this->round = 0;
		this->callAmount = 0;
		this->minRaise = 0;
		this->numberOfRaisesSoFar = 0;
		this->nextBlindShiftDeadlineIndex = 0;

		// subscribe to BroadcastStation
		this->subscribe();
	}

	virtual ~Hostess()
	{
		delete [] this->botsInGame;
		delete [] this->botsInRound;
	}

	void fillBotsData();
	void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data);
	int getCallAmount() const; // UNIT done
	int getMinRaise() const; // UNIT done
	int getBigBlindAtRound(int round) const;
	int getSmallBlindAtRound(int round) const; // UNIT done
	int getNextBlindShiftDeadline() const; // UNIT done
	int getBotIDToTheRight(int fromID, int nth, bool onlyInGame, bool onlyInRound) const; // UNIT done
	int getBotIDToTheLeft(int fromID, int nth, bool onlyInGame, bool onlyInRound) const; // UNIT done
	const BotInfo* getBotByID(int botID) const;
	int getBotIDByIndex(int index) const;
	int getBotIndexByID(int botID) const;
	int getNumOfBots(bool onlyInGame, bool onlyInRound) const;
	int getCurrentRound() const;
	HandRank getBotHandRank(Card* botCard1, Card* botCard2) const;

};

#endif  //_HOSTESS_H
