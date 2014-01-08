#ifndef INTERRUPTABLETHREAD_H
#define INTERRUPTABLETHREAD_H

#include "stdafx.h"
#include "InterruptThreadException.h"
#include "Bot.h"


//forrás: http://stackoverflow.com/questions/13893060/i-want-to-kill-a-stdthread-using-its-thread-object
//isAlive check: http://stackoverflow.com/questions/9094422/how-to-check-if-a-stdthread-is-still-running
class InterruptableThread
{
private:
    std::thread _thread;
    static thread_local std::atomic_bool* _flag_ref;
    std::atomic_bool _flag;

public:
    friend void check_for_interrupt();

    InterruptableThread()
    {
        _flag_ref = nullptr;
        _flag = false;
    }

/*
Így megy
std::packaged_task<void()> task(std::bind(&Bot::roundStarted, this->forwardTo, round));
auto future = task.get_future();

// Run task on new thread.
std::thread t(std::move(task));
if(!this->isInTime(std::move(future)))
{
    this->handleTimeout("roundStarted");
}

*/
    template <typename Function, typename... Args>
    std::future<void>&& Start(Function&& fun, Bot* instance, Args&&... args)
    {
        std::packaged_task<void()> task(
        [] (Bot* instanceA, std::atomic_bool& f, Function&& funA, Args&&... argsA)
            {
                _flag_ref = &f;
                (instanceA->*funA)(std::forward<Args>(argsA)...);
            });
        auto future = task.get_future();

        // Run task on new thread.
        std::thread t(std::move(task),
                    std::forward<Bot*>(instance),
                    _flag,
                    std::forward<Function>(fun),
                    std::forward<Args>(args)...);
    }

    bool Stopping() const { return _flag.load(); }

    void Stop() { _flag.store(true); }
};

inline void check_for_interrupt() noexcept(false) {
    if (not InterruptableThread::_flag_ref) { return; }
    if (not InterruptableThread::_flag_ref->load()) { return; }

    throw InterruptThreadException();
} // check_for_interrupt

#endif // INTERRUPTABLETHREAD_H
