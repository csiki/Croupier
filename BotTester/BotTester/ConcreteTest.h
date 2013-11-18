#if !defined(_CONCRETETEST_H)
#define _CONCRETETEST_H

#include "Test.h"
#include "BotState.h"

class ConcreteTest : public Test
{
public:
    void initGame(BotData* toTest, int gameid);
};

#endif  //_CONCRETETEST_H
