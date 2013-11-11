#if !defined(_TESTER_H)
#define _TESTER_H

#include "BotState.h"
#include "Bot.h"
#include "Log.h"
#include "Test.h"

class Tester
{
public:
    void test(int Bot*);
    Log* retrieveLog();
    BotState retrieveState();
private:
    Bot *subject;
    Log *log;
    BotState *state;
    Test *tests;
};

#endif  //_TESTER_H
