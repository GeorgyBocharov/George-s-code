#ifndef GAME_CONTROLFUNCTIONS_H
#define GAME_CONTROLFUNCTIONS_H

#include "Controllable.h"
#include "EngineFunctions.h"
#include "Bullets.h"

Controllable::Controllable(sf::Texture &texture) {
    sprite_.setTexture(texture);
    width = sprite_.getTexture()->getSize().x;
    height = sprite_.getTexture()->getSize().y;
    id = quantity;


    sprite_.setOrigin(width / 2, height / 2);

    mass = SUP_MASS;
}

void Controllable::dump() {
    engine_->window_.draw(sprite_);
    std::cout << sprite_.getPosition().x << sprite_.getPosition().y << "\n";
    std::cout << coord.x << coord.y << "\n\n";
}

void Controllable::draw() {
    sprite_.setPosition(coord);
    engine_->window_.draw(sprite_);
}

bool Controllable::operator!=(const Controllable &that) {
    return !(*this == that);
}

bool Controllable::operator==(const Controllable &that) {
    return coord.x == that.coord.x && that.coord.y == coord.y;
}

void Controllable::setEngine(Engine *engine) { this->engine_ = engine; }

sf::Vector2f Controllable::GetCoordinates() { return coord; }

sf::Vector2f Controllable::GetDestination() { return dest; }

sf::Vector2f Controllable::collDistance(Controllable &that) { return that.GetCoordinates() - coord; }

bool Colide(Controllable &member) { return false; }

double Controllable::getSize() { return (width + height) / 3.3; }

int Controllable::GetType() { return type; }

bool Controllable::colide(Controllable *that) {
    double length1 = length(this->collDistance(*that));
    auto size = this->collisionRad + that->collisionRad;

    if (length1 < size) {
        if ((this->collisional) && (that->collisional)) {
            double angle = atan(fabs((that->coord.y - this->coord.y) / (that->coord.x - this->coord.x)));

            this->coord.x = (float) (that->coord.x - sgn(that->coord.x - this->coord.x) * (size) *
                                                     cos(angle));

            this->coord.y = (float) (that->coord.y - sgn(that->coord.y - this->coord.y) * (size) *
                                                     sin(angle));
        }
        return true;
    } else
        return false;

}

Engine *Controllable::GetEngine() {
    return engine_;
}

void Character::move() {

    if (sqrt(pow(v.x + a.x, 2) + pow(v.y + a.y, 2)) < vMax) {
        v.x += a.x * engine_->dtime_;
        v.y += a.y * engine_->dtime_;
    }

    if (sqrt(pow(v.x, 2) + pow(v.y, 2)) > vMax) {
        v.y /= sqrt(vMax);
        v.x /= sqrt(vMax);
    }
    coord.x += v.x * engine_->dtime_;
    coord.y += v.y * engine_->dtime_;
}


Character::Character(sf::Texture &texture) : Controllable(texture) {}

void Character::draw() {
    Controllable::draw();
}

void Character::colisionResponse(Character *that) {
    if ((!that->collisional) && (this->collisional)) {
        if ((-(that->type) != this->type) && (that->type > BONUS)) {
            if (type >= ALIVE) {

                std::cout << that->id << " Shoot " << this->id << "\n";
                if (hp - that->dmg > 0)
                    hp -= that->dmg;
                else
                    hp = 0;
            }
            std::cout << "INTERSECTION WITH " << id << "\n";
            engine_->DeleteCharacter(that);
        }
        else if (that->type <= BONUS){
            this->bonus = that->type - BONUS;
            std::cout << "BONUS INTERSECTION " << id << "\n";
            engine_->DeleteCharacter(that);
        }



    } else {
        collisionClock.restart();
        that->collisionClock.restart();


        float deltaP = dot(this->v - that->v, this->collDistance(*that));
        deltaP *= (2 * this->mass * that->mass) / ((this->mass + that->mass) * length(this->collDistance(*that)));


        sf::Vector2f dp = this->collDistance(*that) * (deltaP / 100.f);
        v -= dp / this->mass;
        that->v -= dp / that->mass;


        if (this->type != that->type) {
            this->accident();
            that->accident();
        }
    }


}


void Character::accident() {
    if (type >= ALIVE) {

        if (hp - COL_DMG > 0)
            hp -= COL_DMG;
        else
            hp = 0;
    }
}

void Character::ControlBounds(double x, double y) {

    if (((coord.x > BACK_X - x) && (v.x > 0)) ||
        ((coord.x < x) && (v.x < 0))) {

        sprite_.setPosition(check(coord.x, x, BACK_X), coord.y);
        v.x *= (-1);
    } else if (((coord.y > BACK_Y - y) && (v.y > 0)) ||
               ((coord.y < y) && (v.y < 0))) {

        sprite_.setPosition(coord.x, check(coord.y, y, BACK_Y));
        v.y *= (-1);
    }
}

void Character::attack() {
    if (clock_shot.getElapsedTime().asSeconds() > cooldown) {
        this->Shot();
        clock_shot.restart();

    }
}

void Character::Shot() {
    auto *bullet_ptr = new Bullets(bulletTexture, *this);
}

sf::Vector2f Character::GetVelocity() {
    return v;
}

int Character::GetDamage() {
    return dmg;
}

int Character::GetBonus() {
    return bonus;
}

Spacecraft::Spacecraft(sf::Texture &texture, Engine *engine) : Character(texture) {
    std::cout << "Im at the start of Spacecraft\n";

    cooldown = 0.8;
    hp = SUP_HEALTH;
    type = SPACECRAFT;
    density = 2000;

    dmg = SUP_DAMAGE;

    vMax = 100;

    bulletTexture.loadFromFile("ship_bullet.png");

    engine_ = engine;
    engine_->GetNewCharacter(this);

    coord.x = BACK_X / 2;
    coord.y = BACK_Y / 2;

    sprite_.setScale((textureSize / width), (textureSize / height));

    width = textureSize;
    height = textureSize;

    collisionRad = textureSize / 2.f;
    mass = 4 / 3 * density * PI * powf(collisionRad, 3);

    std::cout << "Im at the end of Spacecraft\n";
}


void Spacecraft::move() { Character::move(); }

void Spacecraft::draw() { Controllable::draw(); }

void Spacecraft::logic() {
    const float accel = 2;
    const float slowdown = 0.01;

    const float EPSILON = 0.0001;
    if (collisionClock.getElapsedTime().asMilliseconds() > 1000) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            a.y = -accel;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            a.x = -accel;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            a.y = accel;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            a.x = accel;

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            a.x = 0;

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            a.y = 0;

        if (fabs(pow(pow(v.x, 2) + pow(v.y, 2), 0.5)) > EPSILON) {/* which means that we're moving*/
            a.x += -sgn(v.x) * slowdown * v.x * v.x;
            a.y += -sgn(v.y) * slowdown * v.y * v.y;
        } else {
            v.x = 0;
            v.y = 0;
        }
    } else a.x = a.y = 0;
/* putting brims of map */
    if (coord.x < WIN_X / 2) {
        v.x = 0;
        a.x = 0;
        coord.x = WIN_X / 2;
    }

    if (coord.x > BACK_X - WIN_X / 2) {
        v.x = 0;
        a.x = 0;
        coord.x = BACK_X - WIN_X / 2;
    }
    if (coord.y < WIN_Y / 2) {
        v.y = 0;
        a.y = 0;
        coord.y = WIN_Y / 2;
    }
    if (coord.y > BACK_Y - WIN_Y / 2) {
        v.y = 0;
        a.y = 0;
        coord.y = BACK_Y - WIN_Y / 2;
    }
    MouseOrientation();

    if (sf::Mouse::isButtonPressed((sf::Mouse::Left))) {
        sf::Vector2i mouse = sf::Mouse::getPosition(engine_->window_);
        mouse.x += coord.x - WIN_X / 2;
        mouse.y += coord.y - WIN_Y / 2;
        dest = sf::Vector2f(mouse.x, mouse.y) - coord;


        attack();
    }

    if ((hp == 0) && (type >= ALIVE)) {
        std::cout << this->id << "\n";
        engine_->DeleteCharacter(this);


    }

}

void Spacecraft::MouseOrientation() {

    sf::Vector2i mouse = sf::Mouse::getPosition(engine_->window_);
    sf::Vector2f center = sprite_.getPosition();

    mouse.x += center.x - WIN_X / 2;
    mouse.y += center.y - WIN_Y / 2;

    sf::Vector2f ptr = sf::Vector2f(mouse.x, mouse.y) - center;
    sprite_.setRotation(270 + atan2f(ptr.y, ptr.x) * 180 / PI);


}

sf::Vector2f Spacecraft::FindViewCenter(const sf::Sprite &background, sf::View &gameView) {
    sf::Vector2f center = sf::Vector2f(sprite_.getPosition().x, sprite_.getPosition().y);

    center.x = std::max(center.x, gameView.getSize().x / 2);
    center.y = std::max(center.y, gameView.getSize().y / 2);

    center.x = std::min(center.x, background.getTexture()->getSize().x - gameView.getSize().x / 2);
    center.y = std::min(center.y, background.getTexture()->getSize().y - gameView.getSize().y / 2);
    return center;
}

void Spacecraft::attack() { Character::attack(); }

void Spacecraft::Shot() { Character::Shot(); }

#endif //GAME_CONTROLFUNCTIONS_H
