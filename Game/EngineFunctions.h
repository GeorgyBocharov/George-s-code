//
// Created by andrew on 24.05.18.
//

#ifndef GAME_ENGINEFUNCTIONS_H
#define GAME_ENGINEFUNCTIONS_H

#include "ControlFunctions.h"
#include "Lifebar.h"
#include "Menu.h"

sf::Sprite Triumph (sf::Vector2f coord);
int Status = 1;

void Engine::GetNewCharacter(Controllable *that) {
    objects[quantity] = that;
    Increment();
}

void Engine::Increment() {
    quantity++;

}

Engine::Engine() :
        window_(sf::VideoMode(WIN_X, WIN_Y), "Play!"),menu_(WIN_X, WIN_Y),
        gameView_(sf::FloatRect(0.0f, 0.0f, WIN_X, WIN_Y)) {

    window_.setView(gameView_);
}


void Engine::DeleteCharacter(Controllable *that) {

    if (that->id != 0) {
        Controllable *temp = objects[quantity - 1];
        objects[that->id] = temp;
        quantity--;
        std::cout << "object number " << that->id << " is deleted" << std::endl;
        std::cout << "quantity is " << quantity << "\n\n" << std::endl;
    }
    else {
        sf::Texture skeleton;
        skeleton.loadFromFile("skull.png");
        sf::Sprite skull(skeleton);
        skull.setOrigin(skeleton.getSize().x / 2.f, skeleton.getSize().y / 2.f);
        skull.setPosition(that->GetCoordinates());
        skull.setScale(2 * 1200 / skeleton.getSize().x, 2 * 700 / skeleton.getSize().y);
        window_.draw(skull);
        Status = 0;
    }
}


void Engine::run() {

    sf::Texture map;
    map.loadFromFile("space.jpg");
    background_.setTexture(map);
    window_.setFramerateLimit(60);

    sf::Event event{};
    while (window_.isOpen()) {
        window_.clear();

        dtime_ = clock_.getElapsedTime().asMicroseconds() / 50000.f;
        clock_.restart();

        window_.draw(background_);

        while (window_.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window_.close();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window_.close();

        sf::Vector2f center = dynamic_cast<Spacecraft *>(objects[0])->FindViewCenter(background_, gameView_);
        gameView_.setCenter(sf::Vector2f(center.x, center.y));
        window_.setView(gameView_);

        if (objects) {
            for (int i = 0; i < quantity; i++) {
                auto object = dynamic_cast <Character *> (objects[i]);
                object->logic();

                for (int j = 1; j < quantity; ++j) {
                    if ((j != i) && (object->colide(objects[j])) &&
                        (objects[j]->status != INTERACTED)) {

                        object->colisionResponse(dynamic_cast<Character *>(objects[j]));
                    }
                }

                if (i != quantity - 1)
                    object->status = INTERACTED;

                else {
                    for (int count = 0; count < quantity; count++)
                        objects[count]->status = DEFAULT;
                }

                if (Status) {
                    object->move();
                    object->draw();
                }

            }
        }
        if (quantity == MET_AM + 1) {
            window_.draw(Triumph(objects[0]->GetCoordinates()));
        }
        window_.display();
    }
    std::cout << "quantaty is " << quantity;
}

void Engine::Start() {
    sf::Texture wallpapers;
    wallpapers.loadFromFile("wallPaper.jpg");
    sf::Sprite wallpaper(wallpapers);

    sf::Texture themes;
    themes.loadFromFile("startBackground.png");
    sf::Sprite theme(themes);
    theme.setScale(0.30,0.3);
    theme.setPosition(-150, 0);
    while (window_.isOpen()) {
        sf::Event event{};

        while (window_.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::KeyReleased:
                    switch (event.key.code) {
                        case sf::Keyboard::Up:
                            menu_.MoveUp();
                            break;

                        case sf::Keyboard::Down:
                            menu_.MoveDown();
                            break;

                        case sf::Keyboard::Return:
                            switch (menu_.GetPressedItem()) {
                                case 0:
                                    this->run();
                                    break;
                                case 1:
                                    std::cout << "Option button has been pressed" << std::endl;
                                    break;
                                case 2:
                                    this->window_.close();
                                    break;
                            }
                            break;
                        case sf::Keyboard::Escape:
                            window_.close();
                            break;
                    }

                    break;
                case sf::Event::Closed:
                    this->window_.close();

                    break;


            }
        }

        window_.draw(wallpaper);
        window_.draw(theme);
        menu_.draw(window_);

        window_.display();
    }

}

sf::Sprite Triumph (sf::Vector2f coord) {
    sf::Texture picture;
    picture.loadFromFile("bonus1.png");
    sf::Sprite win(picture);
    float size = picture.getSize().x;
    win.setOrigin(size / 2, size / 2);
    win.setScale(WIN_X / size, WIN_Y / size);
    win.setPosition(coord);
    return  win;
}

#endif //GAME_ENGINEFUNCTIONS_H
