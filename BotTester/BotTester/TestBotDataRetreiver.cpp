#include "TestBotDataRetreiver.h"

BotData* TestBotDataRetreiver::retrieveNormalBot()
{
    return new BotData(0, 0, "normal", "test_normal.so", 1000, BotLanguage::CPP,
		0, 0);
}

BotData* TestBotDataRetreiver::retrieveShyBot()
{
    return new BotData(1, 0, "shy", "test_shy.so", 1000, BotLanguage::CPP,
		0, 0);
}

BotData* TestBotDataRetreiver::retrieveAggressiveBot()
{
    return new BotData(2, 0, "aggressive", "test_aggressive.so", 1000, BotLanguage::CPP,
		0, 0);
}
