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
	
	virtual void allined(int botID, int amount) {}
	virtual void blindsRaised(int newSmallBlind, int newBigBlind) {}
	virtual void called(int botID, int amount) {}
	virtual void checked(int botID) {}
	virtual void flop() {}
	virtual void folded(int botID) {}
	virtual void gameWinner(int botID) {}
	virtual void leftGame(int botID) {}
	virtual void listen(int botID, Comment comment) {}
	virtual void preflop() {}
	virtual void leave() {}
	virtual void raised(int botID, int amount) {}
	virtual void rebuyOccurred(int botID, int amount) {}
	virtual void rebuyDeadlineReached() {}
	virtual void rebuyOrLeave() {}
	virtual void river() {}
	virtual void roundEnded() {}
	virtual void roundStarted(int round) {}
	virtual void roundWinners(int numOfWinners, const int* winners) {}
	virtual void showdown() {}
	virtual void step() = 0;
	virtual void turn() {}
};

// class factories for loading Bot from object files
typedef Bot* (*create_bot)(BotCommunicator*, int, std::string, BotLanguage);
typedef void (*destroy_bot)(Bot*);

#endif  //_BOT_H