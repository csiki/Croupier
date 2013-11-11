#if !defined(_TESTBOTDATARETREIVER_H)
#define _TESTBOTDATARETREIVER_H

#include "BotData.h"

class TestBotDataRetreiver
{
public:
    static BotData* retrieveNormalBot();
    static BotData* retrieveShyBot();
    static BotData* retrieveAggressiveBot();
};

#endif
