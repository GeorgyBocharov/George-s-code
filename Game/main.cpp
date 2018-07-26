#include "EngineFunctions.h"
#include "Metheors.h"
#include "Enemy.h"
#include "Bonus.h"

#define JUNK rand()

int main() {
    Engine engine;
    int i = JUNK;
    sf::Texture ironMan;
    ironMan.loadFromFile("Falcon.png");
    sf::Sprite sprite(ironMan);

    sf::Texture metheor;
    metheor.loadFromFile("metheor.png");

    sf::Texture bots;
    bots.loadFromFile("friend.png");

    sf::Texture bonus_first;
    bonus_first.loadFromFile("bonus1.png");

    sf::Texture bonus_second;
    bonus_second.loadFromFile("bonus2.png");


    Spacecraft djordj(ironMan, &engine);

    for (i = 0; i < BOT_AMOUNT; i++){
        auto* bot_ptr = new Enemy(bots, &engine);
    }

      for (i = 0; i < MET_AM; i++){
        auto* met_ptr = new Metheors(metheor, &engine);
    }

    engine.Start();
    Gde;

    return 0;
}






