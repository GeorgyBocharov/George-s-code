//
// Created by george on 22.05.18.
//

#ifndef GAME_ENEMY_H
#define GAME_ENEMY_H

#include "ControlFunctions.h"
#include "Bullets.h"

class Enemy : public Character {
    friend Bullets;

protected:
    double viewRange;
    double atackRange;

    const float BOT_SIZE = 64.f;
    const float BOT_CD = 1.5;
    const int BOT_VI = 400;
    const int BOT_AT = 300;
    const int BOT_DMG = 50;
    const int BOT_ST = 4;
    const int BOT_HP = 100;
    const int ACCEL_MAX = 2;



public:
    explicit Enemy(sf::Texture &texture, Engine *engine);

    void logic() override;

    void freeFlight();

    void Orientation() {
        sprite_.setRotation(90 + atan2f(v.y, v.x) * 180 / PI);
    }

    void chasing(sf::Vector2f distance);

    void Shot() override;

    void move() override;

    void attack() override;

    void draw() override;
};

Enemy::Enemy(sf::Texture &texture, Engine *engine) : Character(texture) {

    density = 2000;
    vMin = 2;
    type = BOT;
    vMax = 10;
    cooldown = BOT_CD;
    viewRange = BOT_VI;
    atackRange = BOT_AT;
    hp = BOT_HP;
    stamina = BOT_ST;
    dmg = BOT_DMG;
    sprite_.setScale(BOT_SIZE / width, BOT_SIZE / height);

    bulletTexture.loadFromFile("bot_bullet.png");

    height = BOT_SIZE;
    width = BOT_SIZE;

    coord.x = WIN_X / 2;
    coord.y = BACK_Y / 2 + quantity * sgn(rand() - RAND_MAX/2) * 100;

    v.x = vMin + rand() % (vMax - vMin);
    v.y = sgn(rand() - RAND_MAX / 2) * rand() % (vMax);

    collisionRad = BOT_SIZE / 2.f;

    mass = 4 / 3 * density * PI * powf(collisionRad, 3);

    engine_ = engine;
    engine_->GetNewCharacter(this);

}

void Enemy::draw() {
    Controllable::draw();
}

void Enemy::logic() {

    Controllable *ship = engine_->objects[0];
    dest = ship->GetCoordinates() - coord;
    if (collisionClock.getElapsedTime().asMilliseconds() > 1000) {
        if (length(dest) < BOT_VI)
            chasing(dest);
        if (length(dest) < BOT_AT) {
            chasing(dest);
            attack();
        }
        if (length(dest) >= BOT_VI)
            freeFlight();
    }
    ControlBounds(WIN_X / 2, WIN_Y / 2);
    Orientation();



    if ((hp == 0) && (type >= ALIVE))
        engine_->DeleteCharacter(this);
}

void Enemy::chasing(sf::Vector2f distance) {

    v = distance;
    v.x /= length(distance) / vMax ;
    v.y /= length(distance) / vMax;

}

void Enemy::freeFlight() {
    float angle;
    if (v.x == 0)
        angle = 180;
    else
        angle = atanf(v.y / v.x) + 90;

    a.x = (rand() % ACCEL_MAX) * sgn(rand() - RAND_MAX / 2) * cosf(angle);
    a.y = (rand() % ACCEL_MAX) * sgn(rand() - RAND_MAX / 2) * sinf(angle);

}

void Enemy::attack() { Character::attack();}

void Enemy::Shot() {
    Character::Shot();
//    auto * bullet_ptr = new Bullets (bulletTexture, engine_, coord, distance) ;
}

void Enemy::move() {Character::move();}


#endif //GAME_ENEMY_H
