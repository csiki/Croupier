#if !defined(_BOT_H)
#define _BOT_H

#include "Comment.h"
#include "BotCommunicator.h"

// mivan, ha nem csak egy oszt�ly akar deklar�lni, hanem saj�t adatszerkezeteket stb-ket akar..
// --> t�bb file-t is tud szerkeszteni �s az eg�szet egybe compile-oljuk
// + dekstop bot tester
// TODO : bufferoverflow ellen v�delmet !!!
// rajzolni �br�t ami a j�t�kmenet szerint milyen helyzetekben h�v�dnak meg a f�ggv�nyek (pl. flop() ut�n step() stb.)

/**	Base class for AIs.
*/
class Bot
{
private:
	const int id;
	const string name;
	const BotLanguage lang;
public:

	Bot(int id, string name, BotLanguage lang) : id(id), name(name), lang(lang) {}
	virtual ~Bot() {}

	int getID() const;
	BotLanguage getLang() const;
	std::string getName() const;
	
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
	virtual void rebuyOccurred(int botID, int amount);
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