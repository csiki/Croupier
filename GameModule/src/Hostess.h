#if !defined(_HOSTESS_H)
#define _HOSTESS_H

#include "BroadcastMember.h"
#include "Table.h"
#include "Rulz.h"
#include "BroadcastMessage.h"
#include "BotInfo.h"

// TODO playerek azért fizethetnek, hogy a hostess egyes algoritmusait kihasználhatják (használatonként/havonta)

/**	Helper for AIs.
*/
class Hostess : public BroadcastMember
{
private:
	const Table* table;
	const Rulz* rules;
	map<int, const BotInfo*> botsByID;
	bool* botsInRound; // define at constructor
	bool* botsInGame; // define at constructor
	int numOfBots;
	int round;
	int callAmount;
	int minRaise;
	int numberOfRaisesSoFar;
	int nextBlindShiftDeadlineIndex;

	void handleRaise(int raiseAmount); // UNIT

public:
	/** Create a Hostess instance with id -1.
	*/
	Hostess(const Table* table, const Rulz* rules, BroadcastStation* broadcastStation) :
		BroadcastMember(-1, broadcastStation), Entity(-1)
	{
		this->table = table;
		this->rules = rules;
		this->numOfBots = table->getNumOfBots();

		this->botsInGame = new bool[this->numOfBots];
		this->botsInRound = new bool[this->numOfBots];
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
	int getCallAmount() const; // UNIT
	int getMinRaise() const; // UNIT
	int getBigBlindAtRound(int round) const;
	int getNextBlindShiftDeadline() const; // UNIT
	int getSmallBlindAtRound(int round) const; // UNIT
	int getBotIDToTheRight(int fromID, int nth, bool onlyInGame, bool onlyInRound) const; // UNIT
	int getBotIDToTheLeft(int fromID, int nth, bool onlyInGame, bool onlyInRound) const; // UNIT
	const BotInfo* getBotByID(int botID) const;
	int getBotIDByIndex(int index) const;
	int getBotIndexByID(int botID) const;
	int getNumOfBots(bool onlyInGame, bool onlyInRound) const;
	int getCurrentRound() const; // UNIT
	int getNumberOfRaisesLeft() const; // UNIT
};

#endif  //_HOSTESS_H
