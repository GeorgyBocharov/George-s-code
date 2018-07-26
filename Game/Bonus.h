//
// Created by george on 09.06.18.
//

#ifndef GAME_BONUS_H
#define GAME_BONUS_H


#include "Controllable.h"

class Bonus : public Character {
public:
    explicit Bonus(sf::Texture &texture, Engine *engine, int origin);

    void logic () override;

    void draw() override;

    void move() override;

};

Bonus::Bonus(sf::Texture &texture, Engine *engine, int origin)  : Character(texture) {
    coord.x = (float) WIN_X / 2 + rand() % (BACK_X - 3 * WIN_X / 2);
    coord.y = (float) WIN_Y / 2 + rand() % (BACK_Y - 3 * WIN_Y / 2);
    type = BONUS - origin;
    sprite_.setScale(BONUS_SIZE / width, BONUS_SIZE / height);
    collisionRad = BONUS_SIZE;
    collisional = false;
    engine_ = engine;
    engine_->GetNewCharacter(this);
}

void Bonus::logic () {}
void Bonus::draw() {Controllable::draw();}
void Bonus::move() {}




#endif //GAME_BONUS_H
