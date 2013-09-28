#include "stdafx.h"
#include "TimerBotProxy.h"
#include "BotTimeExceededException.h"

/** Measure the time between the call and the end of process of the thread given.
 *	@return true if the AI process was in time, false otherwise.
*/
bool TimerBotProxy::isInTime(boost::thread& runIn)
{
	// TODO idõtullépés log
	std::clock_t begin = clock();
	std::clock_t now = begin;
	boost::chrono::milliseconds sleepDuration(10);
	bool doneInTime = false;
	
	while ((double(now - begin) / CLOCKS_PER_MILLISEC) < this->allowedCalcTime)
	{
		// cycle till thread is running and till the allowed time not elapsed
		boost::this_thread::sleep_for(sleepDuration);
		now = clock();

		if (!this->isThreadRunning(runIn))
		{
			doneInTime = true;
			break;
		}
	}
	
	// interrupt if running
	if (this->isThreadRunning(runIn))
	{
		runIn.interrupt();
	}

	return doneInTime;
}

void TimerBotProxy::handleTimeout(string inMethod)
{
	throw BotTimeExceededException(inMethod);
}

bool TimerBotProxy::isThreadRunning(boost::thread& runIn)
{
	return !runIn.timed_join(boost::posix_time::milliseconds(0));
}

void TimerBotProxy::allined(int botID, int amount)
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot, botID, amount] () {
		boost::this_thread::interruption_point();
		bot->allined(botID, amount);
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("allined");
	}
}

void TimerBotProxy::blindsRaised(int newSmallBlind, int newBigBlind)
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot, newSmallBlind, newBigBlind] () {
		boost::this_thread::interruption_point();
		bot->blindsRaised(newSmallBlind, newBigBlind);
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("blindsRaised");
	}
}

void TimerBotProxy::called(int botID, int amount)
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot, botID, amount] () {
		boost::this_thread::interruption_point();
		bot->called(botID, amount);
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("called");
	}
}

void TimerBotProxy::checked(int botID)
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot, botID] () {
		boost::this_thread::interruption_point();
		bot->checked(botID);
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("checked");
	}
}

void TimerBotProxy::flop()
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot] () {
		boost::this_thread::interruption_point();
		bot->flop();
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("flop");
	}
}

void TimerBotProxy::folded(int botID)
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot, botID] () {
		boost::this_thread::interruption_point();
		bot->folded(botID);
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("folded");
	}
}

void TimerBotProxy::gameWinner(int botID)
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot, botID] () {
		boost::this_thread::interruption_point();
		bot->gameWinner(botID);
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("gameWinner");
	}
}

void TimerBotProxy::leftGame(int botID)
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot, botID] () {
		boost::this_thread::interruption_point();
		bot->leftGame(botID);
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("leftGame");
	}
}

void TimerBotProxy::listen(int botID, Comment comment)
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot, botID, comment] () {
		boost::this_thread::interruption_point();
		bot->listen(botID, comment);
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("listen");
	}
}

void TimerBotProxy::preflop()
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot] () {
		boost::this_thread::interruption_point();
		bot->preflop();
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("preflop");
	}
}

void TimerBotProxy::leave()
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot] () {
		boost::this_thread::interruption_point();
		bot->leave();
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("leave");
	}
}

void TimerBotProxy::raised(int botID, int amount)
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot, botID, amount] () {
		boost::this_thread::interruption_point();
		bot->raised(botID, amount);
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("raised");
	}
}

void TimerBotProxy::rebuyOccurred(int botID, int amount)
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot, botID, amount] () {
		boost::this_thread::interruption_point();
		bot->rebuyOccurred(botID, amount);
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("rebuyOccurred");
	}
}

void TimerBotProxy::rebuyDeadlineReached()
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot] () {
		boost::this_thread::interruption_point();
		bot->rebuyDeadlineReached();
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("rebuyDeadlineReached");
	}
}

void TimerBotProxy::rebuyOrLeave()
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot] () {
		boost::this_thread::interruption_point();
		bot->rebuyOrLeave();
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("rebuyOrLeave");
	}
}

void TimerBotProxy::river()
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot] () {
		boost::this_thread::interruption_point();
		bot->river();
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("river");
	}
}

void TimerBotProxy::roundEnded()
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot] () {
		boost::this_thread::interruption_point();
		bot->roundEnded();
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("roundEnded");
	}
}

void TimerBotProxy::roundStarted(int round)
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot, round] () {
		boost::this_thread::interruption_point();
		bot->roundStarted(round);
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("roundStarted");
	}
}

void TimerBotProxy::roundWinners(int numOfWinners, const int* winners)
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot, numOfWinners, winners] () {
		boost::this_thread::interruption_point();
		bot->roundWinners(numOfWinners, winners);
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("roundWinners");
	}
}

void TimerBotProxy::showdown()
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot] () {
		boost::this_thread::interruption_point();
		bot->showdown();
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("showdown");
	}
}

void TimerBotProxy::step()
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot] () {
		boost::this_thread::interruption_point();
		bot->step();
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("step");
	}
}

void TimerBotProxy::turn()
{
	Bot* bot = this->forwardTo;
	boost::thread run([bot] () {
		boost::this_thread::interruption_point();
		bot->turn();
		boost::this_thread::interruption_point();
	});

	if(!isInTime(run))
	{
		this->handleTimeout("turn");
	}
}
