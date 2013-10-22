#if !defined(_RESULTS_H)
#define _RESULTS_H

/**	Contains the results of a game.
 *	Contains botid-credit and botid-falloutround pairs.
*/
class Results
{
private:
	int numOfBots;
	int* botIDs;
	int* playersID;
	int* credits;
	int* kicksAtRound;

public:
	Results(int numOfBotsExpected)
	{
		// numofbots=0 (increase when adding bots)
		this->numOfBots = 0;

		// allocate memory
		this->botIDs = new int[numOfBotsExpected];
		this->credits = new int[numOfBotsExpected];
		this->kicksAtRound = new int[numOfBotsExpected];
		this->playersID = new int[numOfBotsExpected];
	}

	virtual ~Results()
	{
		// deallocate memory
		delete [] this->botIDs;
		delete [] this->credits;
		delete [] this->kicksAtRound;
		delete [] this->playersID;
	}

	void addResult(int botID, int playerID, int credit, int kickAtRound);
	int getNumOfPlayers() const;
	int getBotID(int index) const;
	int getPlayerID(int index) const;
	int getCredit(int index) const;
	int getKickAtRound(int index) const;
};

#endif  //_RESULTS_H
