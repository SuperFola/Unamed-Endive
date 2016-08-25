#include <iostream>

#include "game.hpp"

// private
Game::Game() : window(sf::VideoMode(WIN_W, WIN_H), WIN_TITLE, sf::Style::Titlebar | sf::Style::Close), sm()
{
}

int Game::prepare()
{
    // load components here
    this->shape = sf::CircleShape(100.0f);
    this->shape.setFillColor(sf::Color::Green);
    return 0;
}

void Game::resize_window(int nx, int ny)
{
    // do some stuff, like resizing tiles / player ...
    // ...
    this->window.setSize(sf::Vector2u(nx, ny));
}

void  Game::dispatch_events(sf::Event& event)
{
    switch(event.type)
    {
    case sf::Event::KeyPressed:
        this->shape.setFillColor(sf::Color::Blue);
        break;

    default:
        break;
    }
}

void Game::render()
{
    this->window.draw(this->shape);
}

// public
int Game::run()
{
    // load components and variables
    if (this->prepare())
        return 1;  // error while loading

    sf::Event event;
    while (this->window.isOpen())
    {
        // get deltatime
        sf::Time dt = this->clock.restart();

        // dispatch events using a loop
        while (this->window.pollEvent(event))
        {
            // default events
            switch(event.type)
            {
            case sf::Event::Closed:
                this->window.close();
                break;

            default:
                break;
            }

            // dispatching
            this->dispatch_events(event);
        }

        // rendering
        this->window.clear();
        this->render();
        this->window.display();
    }

    return 0;
}
