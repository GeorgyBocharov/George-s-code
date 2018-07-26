//
// Created by george on 22.05.18.
//

#ifndef GAME_METHEORS_H
#define GAME_METHEORS_H

#include "ControlFunctions.h"


class Metheors : public Character {
protected:
    float rad;
    const int minR = 100;
    const int maxR = 200;

public:
    explicit Metheors(sf::Texture &texture, Engine *engine);

    void move() override;

    void attack() override {};

    void Shot() override {};

    void draw() override;

    void logic() override;
};

void Metheors::draw() {Character::draw();}

Metheors::Metheors(sf::Texture &texture, Engine *engine) : Character(texture) {
    std::cout << "im in met constructor" << std::endl;

    type = -1;
    density = 3000;

    rad = minR + rand() % (maxR - minR);
    mass = PI * 4 / 3 * density * powf(rad / 2, 3);

    sprite_.setScale(2 * rad / width, 2 * rad / height);
    height = rad / 2;
    width = rad / 2;

    vMax = 20;
    vMin = 5;

    coord.x = chance() * (BACK_X - WIN_X / 2) + rand() % (WIN_X / 2);
    coord.y = chance() * (BACK_Y - WIN_Y / 2) + rand() % (WIN_Y / 2);

    v.x = (-1) * sgn(coord.x - WIN_X / 2) * (vMin + rand() % (vMax - vMin));
    v.y = (-1) * sgn(coord.y - WIN_Y / 2) * (vMin + rand() % (vMax - vMin));

    a.x = 0;
    a.y = 0;

    collisionRad = rad / 2;
    engine_ = engine;
    engine_->GetNewCharacter(this);
}

void Metheors::move() { Character::move();
}

void Metheors::logic() { Character::ControlBounds(0, 0); }


#endif
