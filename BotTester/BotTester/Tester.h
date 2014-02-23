#if !defined(_TESTER_H)
#define _TESTER_H

#include "Test.h"
#include "Tester.h"
#include "ConcreteTest.h"
#include "BotData.h"
#include <map>

class Tester
{
private:
    /*int playerid;
    int botid;
    int gameid;*/
    std::map<int, Test*> tests;

public:
    Tester()
    {
        tests.insert(std::pair<int, Test*> (0, new ConcreteTest()));
    }

    ~Tester()
    {
        for (std::map<int, Test*>::iterator it = tests.begin(); it != tests.end(); ++it)
        {
            delete it->second;
        }
    }

    void test(BotData* botdata, int testcase, int gameid);
};

#endif  //_TESTER_H
