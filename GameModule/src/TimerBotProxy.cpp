#include "stdafx.h"
#include "TimerBotProxy.h"

/** Measure the time between the call and the end of process of the thread given.
 *	@return true if the AI process was in time, false otherwise.
*/
bool TimerBotProxy::isInTime(boost::thread& runIn)
{
	std::clock_t begin = clock();
	std::clock_t now = begin;
	boost::chrono::milliseconds sleepDuration(50);
	bool doneInTime = false;
	
	while ((double(now - begin) / CLOCKS_PER_MILLISEC) < this->allowedCalcTime)
	{
		// cycle till thread is running and till the allowed time not elapsed
		if (!this->isThreadRunning(runIn))
		{
			doneInTime = true;
			break;
		}

		boost::this_thread::sleep_for(sleepDuration);
		now = clock();
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
	string msg = "Allowed bot calculation time exceeded in method ";
	msg += inMethod;
	msg += "! Bot ID: ";
	msg += to_string(this->getID());
	throw msg; // TODO exception type for all exceptions !!!4
}

bool TimerBotProxy::isThreadRunning(boost::thread& runIn)
{
	return !runIn.timed_join(boost::posix_time::milliseconds(0));
}

void TimerBotProxy::allined(int botID, int amount)
{
	// TODO as step....
}

void TimerBotProxy::blindsRaised(int newSmallBlind, int newBigBlind)
{
}

void TimerBotProxy::called(int botID, int amount)
{
}

void TimerBotProxy::checked(int botID)
{
}

void TimerBotProxy::flop()
{
}

void TimerBotProxy::folded(int botID)
{
}

void TimerBotProxy::gameWinner(int botID)
{
}

void TimerBotProxy::leftGame(int botID)
{
}

void TimerBotProxy::listen(int botID, Comment comment)
{
}

void TimerBotProxy::preflop()
{
}

void TimerBotProxy::leave()
{
}

void TimerBotProxy::raised(int botID, int amount)
{
}

void TimerBotProxy::rebuyOccurred(int botID, int amount)
{
}

void TimerBotProxy::rebuyDeadlineReached()
{
}

void TimerBotProxy::rebuyOrLeave()
{
}

void TimerBotProxy::river()
{
}

void TimerBotProxy::roundEnded()
{
}

void TimerBotProxy::roundStarted(int round)
{
}

void TimerBotProxy::roundWinners(int numOfWinners, const int* winners)
{
}

void TimerBotProxy::showdown()
{
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
}
