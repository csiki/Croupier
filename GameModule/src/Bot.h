#if !defined(_BOT_H)
#define _BOT_H

#include "Comment.h"
#include "BotCommunicator.h"

// mivan, ha nem csak egy osztály akar deklarálni, hanem saját adatszerkezeteket stb-ket akar..
// --> több file-t is tud szerkeszteni és az egészet egybe compile-oljuk
// + dekstop bot tester
// rajzolni ábrát ami a játékmenet szerint milyen helyzetekben hívódnak meg a függvények (pl. flop() után step() stb.)

/**	Base class for AIs.
*/
class Bot
{
private:
	const int id;
	const std::string name;
	const BotLanguage lang;

protected:
	BotCommunicator* const communicator;

public:
	Bot(BotCommunicator* communicator, int id, std::string name, BotLanguage lang)
		: communicator(communicator), id(id), name(name), lang(lang) {}
	virtual ~Bot() {}

	/**	Returns AI's id.
	*/
	int getID() const
	{
		return this->id;
	}

	/** Returns the programming language the AI is written in.
	*/
	BotLanguage getLang() const
	{
		return this->lang;
	}

	/**	Returns AI's name.
	*/
	std::string getName() const
	{
		return this->name;
	}
	
	virtual void allined(int botID, int amount) = 0;
	virtual void blindsRaised(int newSmallBlind, int newBigBlind) = 0;
	virtual void called(int botID, int amount) = 0;
	virtual void checked(int botID) = 0;
	virtual void flop() = 0;
	virtual void folded(int botID) = 0;
	virtual void gameWinner(int botID) = 0;
	virtual void leftGame(int botID) = 0;
	virtual void listen(int botID, Comment comment) = 0;
	virtual void preflop() = 0;
	virtual void leave() = 0;
	virtual void raised(int botID, int amount) = 0;
	virtual void rebuyOccurred(int botID, int amount) = 0;
	virtual void rebuyDeadlineReached() = 0;
	virtual void rebuyOrLeave() = 0;
	virtual void river() = 0;
	virtual void roundEnded() = 0;
	virtual void roundStarted(int round) = 0;
	virtual void roundWinners(int numOfWinners, const int* winners) = 0;
	virtual void showdown() = 0;
	virtual void step() = 0;
	virtual void turn() = 0;
};

#endif  //_BOT_H