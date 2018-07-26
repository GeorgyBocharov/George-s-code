//
// Created by andrew on 26.05.18.
//

#ifndef GAME_LIFEBAR_H
#define GAME_LIFEBAR_H

#include "Lifebar_h.h"

LifeBar::LifeBar() {
    lifeTexture.loadFromFile("lifeBar2.png");
    lifeSprite_.setTexture(lifeTexture);
    bar.setSize(sf::Vector2f(100,47));
    bar.setFillColor(sf::Color::Green);

}

void LifeBar::draw(sf::RenderWindow &window) {
    sf::Vector2f center = window.getView().getCenter();

    lifeSprite_.setPosition(center.x + 600 / 2 ,center.y  + 500/2);
    bar.setPosition(center.x + 600 / 2 +5 ,center.y  + 500/2 + 14);



    window.draw(bar);
    window.draw(lifeSprite_);
}

void LifeBar::update(int health) {
    sf::Clock clock1;
    if ((health > 0) && (health < max))
        bar.setSize(sf::Vector2f(100 -clock1.getElapsedTime().asMicroseconds()/ 100 , 47  ));
}

#endif //GAME_LIFEBAR_H
