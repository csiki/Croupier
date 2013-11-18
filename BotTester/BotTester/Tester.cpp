#include "Tester.h"
#include "BotState.h"

void Tester::test(BotData* botdata, int testcase, int gameid)
{
    tests[testcase % tests.size()]->initGame(botdata, gameid);
}
