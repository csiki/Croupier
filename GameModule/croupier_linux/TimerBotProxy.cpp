#include "stdafx.h"
#include "TimerBotProxy.h"
#include "BotTimeExceededException.h"
#include "InterruptableThread.h"

/** Measure the time between the call and the end of process of the thread given.
 *	@return true if the AI process was in time, false otherwise.
*/
bool TimerBotProxy::isInTime(std::future<void>&& f)
{
    using namespace std::chrono;

	auto start = high_resolution_clock::now();

	long dur = 0;
	while (dur < this->allowedCalcTime)
	{
		if (f.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
		{
			return true;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		auto now = std::chrono::high_resolution_clock::now();
		dur = duration_cast<milliseconds>(now - start).count();
	}

	return false;
}

void TimerBotProxy::handleTimeout(std::string inMethod)
{
	throw BotTimeExceededException(inMethod);
}

void TimerBotProxy::allined(int botID, int amount)
{
	std::future<void> f = std::async(std::launch::async, &Bot::allined, this->forwardTo, botID, amount);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("allined");
	}
}

void TimerBotProxy::blindsRaised(int newSmallBlind, int newBigBlind)
{
	std::future<void> f = std::async(std::launch::async, &Bot::blindsRaised, this->forwardTo, newSmallBlind, newBigBlind);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("blindsRaised");
	}
}

void TimerBotProxy::called(int botID, int amount)
{
	std::future<void> f = std::async(std::launch::async, &Bot::called, this->forwardTo, botID, amount);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("called");
	}
}

void TimerBotProxy::checked(int botID)
{
	std::future<void> f = std::async(std::launch::async, &Bot::checked, this->forwardTo, botID);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("checked");
	}
}

void TimerBotProxy::flop()
{
	std::future<void> f = std::async(std::launch::async, &Bot::flop, this->forwardTo);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("flop");
	}
}

void TimerBotProxy::folded(int botID)
{
	std::future<void> f = std::async(std::launch::async, &Bot::folded, this->forwardTo, botID);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("folded");
	}
}

void TimerBotProxy::gameWinner(int botID)
{
	std::future<void> f = std::async(std::launch::async, &Bot::gameWinner, this->forwardTo, botID);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("gameWinner");
	}
}

void TimerBotProxy::leftGame(int botID)
{
	std::future<void> f = std::async(std::launch::async, &Bot::leftGame, this->forwardTo, botID);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("leftGame");
	}
}

void TimerBotProxy::listen(int botID, Comment comment)
{
	std::future<void> f = std::async(std::launch::async, &Bot::listen, this->forwardTo, botID, comment);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("listen");
	}
}

void TimerBotProxy::preflop()
{
	std::future<void> f = std::async(std::launch::async, &Bot::preflop, this->forwardTo);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("preflop");
	}
}

void TimerBotProxy::leave()
{
	std::future<void> f = std::async(std::launch::async, &Bot::leave, this->forwardTo);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("leave");
	}
}

void TimerBotProxy::raised(int botID, int amount)
{
	std::future<void> f = std::async(std::launch::async, &Bot::raised, this->forwardTo, botID, amount);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("raised");
	}
}

void TimerBotProxy::rebuyOccurred(int botID, int amount)
{
	std::future<void> f = std::async(std::launch::async, &Bot::rebuyOccurred, this->forwardTo, botID, amount);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("rebuyOccured");
	}
}

void TimerBotProxy::rebuyDeadlineReached()
{
	std::future<void> f = std::async(std::launch::async, &Bot::rebuyDeadlineReached, this->forwardTo);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("rebuyDeadlineReached");
	}
}

void TimerBotProxy::rebuyOrLeave()
{
	std::future<void> f = std::async(std::launch::async, &Bot::rebuyOrLeave, this->forwardTo);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("rebuyOrLeave");
	}
}

void TimerBotProxy::river()
{
	std::future<void> f = std::async(std::launch::async, &Bot::river, this->forwardTo);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("river");
	}
}

void TimerBotProxy::roundEnded()
{
	std::future<void> f = std::async(std::launch::async, &Bot::roundEnded, this->forwardTo);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("roundEnded");
	}
}

void TimerBotProxy::roundStarted(int round)
{
	std::future<void> f = std::async(std::launch::async, &Bot::roundStarted, this->forwardTo, round);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("roundStarted");
	}
}

void TimerBotProxy::roundWinners(int numOfWinners, const int* winners)
{
	std::future<void> f = std::async(std::launch::async, &Bot::roundWinners, this->forwardTo, numOfWinners, winners);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("roundWinners");
	}
}

void TimerBotProxy::showdown()
{
	std::future<void> f = std::async(std::launch::async, &Bot::showdown, this->forwardTo);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("showdown");
	}
}

void TimerBotProxy::step()
{
    InterruptableThread it;
    auto f = it.Start(&Bot::turn, this->forwardTo);
	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("step");
	}
}

void TimerBotProxy::turn()
{
	std::future<void> f = std::async(std::launch::async, &Bot::turn, this->forwardTo);

	if(!this->isInTime(std::move(f)))
	{
		this->handleTimeout("turn");
	}
}
