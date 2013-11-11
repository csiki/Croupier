#if !defined(_TEST_H)
#define _TEST_H

#include "BotState.h"
#include "BotData.h"

class Test
{
public:
    virtual void initGame(BotData* botdata, int gameid) = 0;
};

#endif  //_TEST_H
