#include "ConcreteTest.h"
#include "BotState.h"
#include "TestBotDataRetreiver.h"
#include "BotDataXMLHandler.h"
#include "GameDataXMLHandler.h"
#include <string>

void ConcreteTest::initGame(BotData* botdata, int gameid)
{
    BotData* shybot = TestBotDataRetreiver::retrieveShyBot();
    BotData* aggbot = TestBotDataRetreiver::retrieveAggressiveBot();
    BotData* normalbot = TestBotDataRetreiver::retrieveNormalBot();

    std::string logandresultsfilename = std::to_string(gameid);
    logandresultsfilename += ".xml";

    GameData gamedata(gameid, logandresultsfilename.c_str(), "testrules.xml", logandresultsfilename.c_str());

    std::string xmlpath = "../data/games/";
    xmlpath += std::to_string(gameid);
    xmlpath += ".xml";
    GameDataXMLHandler::saveXML(&gamedata, xmlpath);

    delete shybot;
    delete aggbot;
    delete normalbot;
}

