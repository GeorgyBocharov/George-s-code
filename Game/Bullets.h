//
// Created by george on 24.05.18.
//

#ifndef GAME_BULLETS_H
#define GAME_BULLETS_H


#include "ControlFunctions.h"

class Bullets : public Character {
protected:


    const float bullet_size = 40;
public:
    explicit Bullets(sf::Texture &texture, Character &Batya);

    void logic() override;

    void draw() override;

    void move() override;

    void Orientation();

};

Bullets::Bullets(sf::Texture &texture, Character &Batya) : Character(texture) {
    type = -Batya.GetType();
    hp = -1;
    mass = 0;

    vMax = 40;
    coord = Batya.GetCoordinates();
    //v = distance;
    a.x = a.y = 0;
    collisionRad = 10;
    collisional = false;

    v.x = (Batya.GetDestination().x / length(Batya.GetDestination())) * vMax;
    v.y = (Batya.GetDestination().y / length(Batya.GetDestination())) * vMax;

    dmg = Batya.GetDamage();
    engine_ = Batya.GetEngine();
    engine_->GetNewCharacter(this);

    v.x /= 1.2;
    v.y /= 1.2;

    sprite_.setScale(bullet_size / width, bullet_size / height);
    Orientation();
//    std::cout << "\n\n*****************bullet was created\n\n" << std::endl;
//    std::cout << "\n\n======== MOUSE SPEED ========  " << length(v) << std::endl;
}

void Bullets::logic() {}

void Bullets::draw() {    Character::draw();}

void Bullets::move() {    Character::move();}

void Bullets::Orientation() {
    sprite_.setRotation(180 + atan2f(v.y, v.x) * 180 / PI);
}

#endif //GAME_BULLETS_H
