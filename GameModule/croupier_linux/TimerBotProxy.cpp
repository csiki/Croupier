#include "stdafx.h"
#include "TimerBotProxy.h"
#include "BotTimeExceededException.h"

void TimerBotProxy::runWithTimeout(const std::string& method, const std::function<void()>& fun)
{
  std::atomic_bool exited(false);

  auto lambda = [&]()
     {
         pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
         fun();
         exited.store(true);
     };

  std::thread t(lambda);

  auto start = std::chrono::high_resolution_clock::now();

  long dur = 0;
  while (dur < this->allowedCalcTime)
  {
    if (exited.load())
    {
      t.join();
      return;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    auto now = std::chrono::high_resolution_clock::now();
    dur = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
  }

  // Timeout occurred
  std::lock_guard<std::recursive_mutex> lock(manage_thread_mutex);
  pthread_t id = t.native_handle();
  t.detach();
  pthread_cancel(id);
  throw BotTimeExceededException(method);
  std::cerr << "Timeout " << method << std::endl;
}

void TimerBotProxy::allined(int botID, int amount)
{
  this->runWithTimeout("allined", [&](){this->forwardTo->allined(botID, amount);});
}

void TimerBotProxy::blindsRaised(int newSmallBlind, int newBigBlind)
{
  this->runWithTimeout("blindsRaised", [&](){this->forwardTo->blindsRaised(newSmallBlind, newBigBlind);});
}

void TimerBotProxy::called(int botID, int amount)
{
  this->runWithTimeout("called", [&](){this->forwardTo->called(botID, amount);});
}

void TimerBotProxy::checked(int botID)
{
  this->runWithTimeout("checked", [&](){this->forwardTo->checked(botID);});
}

void TimerBotProxy::flop()
{
  this->runWithTimeout("flop", [&](){this->forwardTo->flop();});
}

void TimerBotProxy::folded(int botID)
{
  this->runWithTimeout("folded", [&](){this->forwardTo->folded(botID);});
}

void TimerBotProxy::gameWinner(int botID)
{
  this->runWithTimeout("gameWinner", [&](){this->forwardTo->gameWinner(botID);});
}

void TimerBotProxy::leftGame(int botID)
{
  this->runWithTimeout("leftGame", [&](){this->forwardTo->leftGame(botID);});
}

void TimerBotProxy::listen(int botID, Comment comment)
{
  this->runWithTimeout("listen", [&](){this->forwardTo->listen(botID, comment);});
}

void TimerBotProxy::preflop()
{
  this->runWithTimeout("preflop", [&](){this->forwardTo->preflop();});
}

void TimerBotProxy::leave()
{
  this->runWithTimeout("leave", [&](){this->forwardTo->leave();});
}

void TimerBotProxy::raised(int botID, int amount)
{
  this->runWithTimeout("raised", [&](){this->forwardTo->raised(botID, amount);});
}

void TimerBotProxy::rebuyOccurred(int botID, int amount)
{
  this->runWithTimeout("rebuyOccurred", [&](){this->forwardTo->rebuyOccurred(botID, amount);});
}

void TimerBotProxy::rebuyDeadlineReached()
{
  this->runWithTimeout("rebuyDeadlineReached", [&](){this->forwardTo->rebuyDeadlineReached();});
}

void TimerBotProxy::rebuyOrLeave()
{
  this->runWithTimeout("rebuyOrLeave", [&](){this->forwardTo->rebuyOrLeave();});
}

void TimerBotProxy::river()
{
  this->runWithTimeout("river", [&](){this->forwardTo->river();});
}

void TimerBotProxy::roundEnded()
{
  this->runWithTimeout("roundEnded", [&](){this->forwardTo->roundEnded();});
}

void TimerBotProxy::roundStarted(int round)
{
  this->runWithTimeout("roundStarted", [&](){this->forwardTo->roundStarted(round);});
}

void TimerBotProxy::roundWinners(int numOfWinners, const int* winners)
{
  this->runWithTimeout("roundWinners", [&](){this->forwardTo->roundWinners(numOfWinners, winners);});
}

void TimerBotProxy::showdown()
{
  this->runWithTimeout("showdown", [&](){this->forwardTo->showdown();});
}

void TimerBotProxy::step()
{
  this->runWithTimeout("step", [&](){this->forwardTo->step();});
}

void TimerBotProxy::turn()
{
  this->runWithTimeout("turn", [&](){this->forwardTo->turn();});
}
