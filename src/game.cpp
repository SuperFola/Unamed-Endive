#include <iostream>
#include <string>

#include "game.hpp"
#include "abstract/functions.hpp"

// private
Game::Game() : window(sf::VideoMode(WIN_W, WIN_H), WIN_TITLE, sf::Style::Titlebar | sf::Style::Close), sm()
{
    // we only add the id of the views
    this->sm.add_view(DEFAULT_VIEW_ID);
}

void Game::resize_window(int nx, int ny)
{
    // do some stuff, like resizing tiles / player ...
    // ...
    this->window.setSize(sf::Vector2u(nx, ny));
}

void  Game::dispatch_events(sf::Event& event)
{
    int c_view = this->sm.getId();

    if (c_view != -1)  // we check if the view exist
    {
        int new_view = UNREACHABLE_VIEW_ID;

        switch (c_view)
        {
        case DEFAULT_VIEW_ID:
            new_view = this->def_view.process_event(event);
            break;

        default:
            break;
        }

        if (this->sm.change_view(new_view))
        {
            // an error occured
            fprintf(stderr, "Unable to find the view %i", new_view);
        }
    }
}

void Game::render()
{
    int c_view = this->sm.getId();

    if (c_view != -1) // does the view exist ?
    {
        switch (c_view)
        {
        case DEFAULT_VIEW_ID:
            this->def_view.render(this->window);
            break;

        default:
            break;
        }
    }
    else
    {
        fprintf(stderr, "Unable to find the view %i", c_view);
    }
}

// public
int Game::run()
{
    sf::Event event;
    while (this->window.isOpen())
    {
        // get deltatime
        sf::Time dt = this->clock.restart();
        float fps = 1.0f / (dt.asSeconds());
        this->window.setTitle(WIN_TITLE + " " + to_string(fps));

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
