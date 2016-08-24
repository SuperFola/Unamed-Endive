//#include <SFML/Graphics.hpp>

#include "game.hpp"

Game::Game() : window(sf::VideoMode(WIN_W, WIN_H), WIN_TITLE)
{
}

int Game::run()
{
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (this->window.isOpen())
    {
        sf::Event event;
        while (this->window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                this->window.close();
        }

        this->window.clear();
        this->window.draw(shape);
        this->window.display();
    }

    return 0;
}
