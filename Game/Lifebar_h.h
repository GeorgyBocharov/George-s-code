//
// Created by andrew on 26.05.18.
//

#ifndef GAME_LIFEBAR_H_H
#define GAME_LIFEBAR_H_H

#include <SFML/Graphics.hpp>

class LifeBar {
public:
    sf::Image image;
    sf::Texture lifeTexture;
    sf::Sprite lifeSprite_;
    int max = 100;
    sf::RectangleShape bar;

    LifeBar();

    void update(int health);

    void draw(sf::RenderWindow &window);

};

#endif //GAME_LIFEBAR_H_H
