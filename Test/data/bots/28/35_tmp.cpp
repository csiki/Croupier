#include "Bot.h"
class PeldaBot : public Bot // leöröklünk Bot osztályból, így felülírhatjuk a metódusait
{
public:
    PeldaBot(BotCommunicator* communicator, int id, std::string name, BotLanguage lang)
        : Bot(communicator, id, name, lang)
    {
        // konstruktor: fejléce bottól függetlenül maradjon ez,
        // itt inicializálhatsz a botod számára tagváltozókat,
        // a bot létrejöttekor ez fut le
    }
 
    void step()
    {
        // így felülírod a step függvényt (az egyetlen amit kötelező is)
        // ide írd a botod döntéseit arra vonatkozólag, hogy mit lépjen mikor ő "beszél"
        if (communicator->getHandRank() >= HandRank::Pair)
        {
            /* ha már egy pár vagy annál jobb lapjai vannak
               (az asztalon lévőkkel együtt nézve persze),
               aggresszív üzemmódba vált:
               emel ha tud
               meghív ha tud
               csekkel ha tud
               minden más esetben allin-ol */
            if (communicator->canRaise(communicator->getBigBlindAtRound()))
                communicator->raise(communicator->getBigBlindAtRound());
            else if (communicator->canCall())
                communicator->call();
            else if (communicator->canCheck())
                communicator->check();
            else
                communicator->allin();
            // a communicator egy pointer ami a BotCommunicator interface-re mutat
            // egy metódusát így tudod meghívni: communicator->metodusNeve(arg1, arg2, ...);
        }
        else
        {
            /* ha párja sincs, akkor jámbor taktikára vált
             csekkel ha tud
             meghív, ha a meghívandó összeg, amit még
             be kell pakolnia, kevesebb mint a zsetonjainak fele
             egyébként bedobja a lapjait */
            if (communicator->canCheck())
                communicator->check();
            else if (communicator->getCallAmount() < communicator->getChips() / 2)
                communicator->call();
            else
                communicator->fold();
        }
    }
 
};
extern "C" Bot* create(BotCommunicator* communicator, int id, std::string name, BotLanguage lang){return new PeldaBot(communicator, id, name, lang);}extern "C" void destroy(Bot* bot){delete bot;}