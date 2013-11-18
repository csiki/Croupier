#if !defined(_TEST_H)
#define _TEST_H

#include "BotState.h"

class Test
{
public:
    virtual BotState test(Bot* bot, Log* log) = 0;
};

#endif  //_TEST_H
