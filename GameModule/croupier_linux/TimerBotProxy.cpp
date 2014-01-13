#include "stdafx.h"
#include "TimerBotProxy.h"
#include "BotTimeExceededException.h"

/** Measure the time between the call and the end of process of the thread given.
 *	@return true if the AI process was in time, false otherwise.
*/
bool TimerBotProxy::isInTime(std::atomic_bool& exited)
{
    using namespace std::chrono;

	auto start = high_resolution_clock::now();

	long dur = 0;
	while (dur < this->allowedCalcTime)
	{
		if (exited.load())
		{
			return true;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		auto now = std::chrono::high_resolution_clock::now();
		dur = duration_cast<milliseconds>(now - start).count();
	}

    timeout_occured.store(true);

	return false;
}

void TimerBotProxy::handleTimeout(std::string inMethod)
{
    timeout_occured.store(false);
	throw BotTimeExceededException(inMethod);
}

void TimerBotProxy::allined(int botID, int amount)
{
	auto lambda = [](std::atomic_bool& exited, Bot* bot, int botID, int amount)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->allined(botID, amount);
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo, botID, amount);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("allined");
	}
    else
        t.join();
}

void TimerBotProxy::blindsRaised(int newSmallBlind, int newBigBlind)
{
	auto lambda = [](std::atomic_bool& exited, Bot* bot, int newSmallBlind, int newBigBlind)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->blindsRaised(newSmallBlind, newBigBlind);
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo, newSmallBlind, newBigBlind);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("blindsRaised");
	}
    else
        t.join();
}

void TimerBotProxy::called(int botID, int amount)
{
	auto lambda = [](std::atomic_bool& exited, Bot* bot, int botID, int amount)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->called(botID, amount);
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo, botID, amount);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("called");
	}
    else
        t.join();
}

void TimerBotProxy::checked(int botID)
{
	auto lambda = [](std::atomic_bool& exited, Bot* bot, int botID)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->checked(botID);
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo, botID);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("checked");
	}
    else
        t.join();
}

void TimerBotProxy::flop()
{
    auto lambda = [](std::atomic_bool& exited, Bot* bot)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->flop();
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("flop");
	}
    else
        t.join();
}

void TimerBotProxy::folded(int botID)
{
	auto lambda = [](std::atomic_bool& exited, Bot* bot, int botID)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->folded(botID);
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo, botID);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("folded");
	}
    else
        t.join();
}

void TimerBotProxy::gameWinner(int botID)
{
	auto lambda = [](std::atomic_bool& exited, Bot* bot, int botID)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->gameWinner(botID);
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo, botID);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("gameWinner");
	}
    else
        t.join();
}

void TimerBotProxy::leftGame(int botID)
{
	auto lambda = [](std::atomic_bool& exited, Bot* bot, int botID)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->leftGame(botID);
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo, botID);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("leftGame");
	}
    else
        t.join();
}

void TimerBotProxy::listen(int botID, Comment comment)
{
	auto lambda = [](std::atomic_bool& exited, Bot* bot, int botID, Comment comment)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->listen(botID, comment);
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo, botID, comment);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("listen");
	}
    else
        t.join();
}

void TimerBotProxy::preflop()
{
    auto lambda = [](std::atomic_bool& exited, Bot* bot)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->preflop();
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("preflop");
	}
    else
        t.join();
}

void TimerBotProxy::leave()
{
    auto lambda = [](std::atomic_bool& exited, Bot* bot)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->leave();
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("leave");
	}
    else
        t.join();
}

void TimerBotProxy::raised(int botID, int amount)
{
	auto lambda = [](std::atomic_bool& exited, Bot* bot, int botID, int amount)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->raised(botID, amount);
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo, botID, amount);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("raised");
	}
    else
        t.join();
}

void TimerBotProxy::rebuyOccurred(int botID, int amount)
{
	auto lambda = [](std::atomic_bool& exited, Bot* bot, int botID, int amount)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->rebuyOccurred(botID, amount);
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo, botID, amount);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("rebuyOccurred");
	}
    else
        t.join();
}

void TimerBotProxy::rebuyDeadlineReached()
{
    auto lambda = [](std::atomic_bool& exited, Bot* bot)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->rebuyDeadlineReached();
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("rebuyDeadlineReached");
	}
    else
        t.join();
}

void TimerBotProxy::rebuyOrLeave()
{
    auto lambda = [](std::atomic_bool& exited, Bot* bot)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->rebuyOrLeave();
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("rebuyOrLeave");
	}
    else
        t.join();
}

void TimerBotProxy::river()
{
    auto lambda = [](std::atomic_bool& exited, Bot* bot)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->river();
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("river");
	}
    else
        t.join();
}

void TimerBotProxy::roundEnded()
{
    auto lambda = [](std::atomic_bool& exited, Bot* bot)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->roundEnded();
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("roundEnded");
	}
    else
        t.join();
}

void TimerBotProxy::roundStarted(int round)
{
	auto lambda = [](std::atomic_bool& exited, Bot* bot, int round)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->roundStarted(round);
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo, round);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("roundStarted");
	}
	else
        t.join();
}

void TimerBotProxy::roundWinners(int numOfWinners, const int* winners)
{
	auto lambda = [](std::atomic_bool& exited, Bot* bot, int numOfWinners, const int* winners)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->roundWinners(numOfWinners, winners);
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo, numOfWinners, winners);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("roundWinners");
	}
    else
        t.join();
}

void TimerBotProxy::showdown()
{
    auto lambda = [](std::atomic_bool& exited, Bot* bot)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->showdown();
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("showdown");
	}
    else
        t.join();
}

void TimerBotProxy::step()
{
    auto lambda = [](std::atomic_bool& exited, Bot* bot)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->step();
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("step");
	}
    else
        t.join();
}

void TimerBotProxy::turn()
{
	 auto lambda = [](std::atomic_bool& exited, Bot* bot)
    {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        bot->turn();
        exited.store(true);
    };

    std::atomic_bool exited(false);

    std::thread t(lambda, ref(exited), this->forwardTo);

	if(!this->isInTime(exited))
	{
        pthread_t id = t.native_handle();
        while (this->critical_to_thread_cancel.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t.detach();
        pthread_cancel(id);
		this->handleTimeout("turn");
	}
    else
        t.join();
}
