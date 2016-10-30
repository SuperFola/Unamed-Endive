#include <iostream>
#include <string>

#include "game.hpp"
#include "abstract/functions.hpp"
#include "abstract/creatures_loader.hpp"

// private
void Game::resize_window(int nx, int ny)
{
    // do some stuff, like resizing tiles / player ...
    // ...
    this->window.setSize(sf::Vector2u(nx, ny));
}

void  Game::dispatch_events(sf::Event& event, sf::Time elapsed)
{
    int c_view = this->sm.getId();

    if (c_view != -1)  // we check if the view exist
    {
        int new_view = UNREACHABLE_VIEW_ID;

        switch (c_view)
        {
        case DEFAULT_VIEW_ID:
            new_view = this->def_view.process_event(event, elapsed);
            break;

        default:
            break;
        }

        if (this->sm.change_view(new_view))
        {
            // an error occured
            std::cout << "Unable to find the view " << new_view << " to process the events" << std::endl;
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
        // error
        std::cout << "Unable to find the view " << c_view << " to render it" << std::endl;
    }
}

void Game::update(sf::Time elapsed)
{
    int c_view = this->sm.getId();

    if (c_view != -1) // does the view exist ?
    {
        switch (c_view)
        {
        case DEFAULT_VIEW_ID:
            this->def_view.update(this->window, elapsed);
            break;

        default:
            break;
        }
    }
    else
    {
        // error
        std::cout << "Unable to find the view " << c_view << " to update it" << std::endl;
    }
}

void Game::update_fps(sf::Time dt, int& _fps_update)
{
    float fps = 1.0f / (dt.asSeconds());
    _fps_update++;
    if (_fps_update % 200 == 0 && DISPLAY_FPS_IN_TITLE)
    {
        this->window.setTitle(WIN_TITLE + to_string(fps));
        _fps_update = 0;
    }
}

// public
Game::Game() :
    window(sf::VideoMode(WIN_W, WIN_H), WIN_TITLE, sf::Style::Titlebar | sf::Style::Close)
    , sm()
{
    // init all the views
    std::cout << "creating views" << std::endl;
    DefaultView def_view = {};
    this->def_view.load();
    std::cout << "views created" << std::endl;

    // we only add the id of the views
    this->sm.add_view(DEFAULT_VIEW_ID);

    // creating base folders
    system("mkdir saves");
    system("mkdir screenshots");
}

int Game::run()
{
    int _fps_update {0};
    // testing
    // CreaturesLoader crea_load {};
    // crea_load.load();
    // end testing

    sf::Event event;
    while (this->window.isOpen())
    {
        // get deltatime
        sf::Time dt = this->clock.restart();

        // update the current view
        this->update(dt);

        // update FPS and display
        this->update_fps(dt, _fps_update);

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
            this->dispatch_events(event, dt);
        }

        // rendering
        this->window.clear();
        this->render();
        this->window.display();
    }

    return 0;
}
