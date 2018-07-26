//
// Created by andrew on 26.05.18.
//

#ifndef GAME_MENU_H_H
#define GAME_MENU_H_H

#include "SFML/Graphics.hpp"

#define MAX_NUMBER_OF_ITEMS 3
class Engine;
class Menu {
public:
    Menu(float width, float height);

    ~Menu() = default;

    void draw(sf::RenderWindow &window);

    void MoveUp();

    void MoveDown();

    int GetPressedItem() { return selectedItemIndex; }


private:
    int selectedItemIndex;
    sf::Font font;
    sf::Text menu[MAX_NUMBER_OF_ITEMS];

};

#endif //GAME_MENU_H_H
