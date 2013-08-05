#if !defined(_HOSTESS_H)
#define _HOSTESS_H

#include "BroadcastMember.h"
#include "Table.h"
#include "Rulz.h"
#include "BroadcastMessage.h"
#include "BotInfo.h"

// TODO playerek azért fizethetnek, hogy a hostess egyes algoritmusait kihasználhatják (használatonként/havonta)
// !TODO legyen numOfBots változója, és ne a tablejét kérje le mindig

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

	void fillBotsData();
	void handleRaise(int raiseAmount);

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
		
		this->fillBotsData();
	}

	virtual ~Hostess()
	{
		delete [] this->botsInGame;
		delete [] this->botsInRound;
	}

	void receiveBroadcast(int fromID, BroadcastMessage msg, int dataSize, const int* data);
	int getCallAmount() const;
	int getMinRaise() const;
	int getBigBlindAtRound(int round) const;
	int getNextBlindShiftDeadline() const;
	int getSmallBlindAtRound(int round) const;
	int getBotIDToTheRight(int fromID, int nth, bool onlyInGame, bool onlyInRound) const;
	int getBotIDToTheLeft(int fromID, int nth, bool onlyInGame, bool onlyInRound) const;
	const BotInfo* getBotByID(int botID) const;
	int getBotIDByIndex(int index) const;
	int getBotIndexByID(int botID) const;
	int getNumOfBots(bool onlyInGame, bool onlyInRound) const;
	int getCurrentRound() const;
	int getNumberOfRaisesLeft() const;
};

#endif  //_HOSTESS_H
