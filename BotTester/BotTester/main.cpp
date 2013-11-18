#include <iostream>
#include <stdlib.h>

#include "Tester.h"
#include "BotData.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        return -1;
    }

    // create BotData object from arguments
    int playerid = atoi(argv[1]);
    int botid = atoi(argv[2]);
    int testcase = atoi(argv[3]);
    int gameid = atoi(argv[4]);
    const char* name = argv[5];
    const char* src = argv[6];
    BotLanguage lang = static_cast<BotLanguage> (atoi(argv[7]));
    int numOfKnowledgeTables = atoi(argv[8]);
    int* knowledgeTables = new int[numOfKnowledgeTables];
    for (size_t i = 0; i < numOfKnowledgeTables; ++i)
    {
        knowledgeTables[i] = atoi(argv[9 + i]);
    }

    BotData botdata(botid, playerid, std::string(name), std::string(src), 1000, lang,
		numOfKnowledgeTables, knowledgeTables);

    Tester tester;
    tester.test(&botdata, testcase, gameid);

    delete [] knowledgeTables;

    return gameid;
}
