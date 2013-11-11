#if !defined(_CONCRETETEST_H)
#define _CONCRETETEST_H

#include "Test.h"
#include "BotState.h"

class ConcreteTest : public Test
{
public:
    BotState test(Bot* bot, Log* log);
};

#endif  //_CONCRETETEST_H
