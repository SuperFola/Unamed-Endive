#include <iostream>
#include <string>

#include "game.hpp"

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
    // we give the current event to the scripting engine if a script need it
    PyScripting::setEvent(event);
    PyScripting::run_event_modules();

    if (c_view != -1)  // we check if the view exist
    {
        int new_view = this->sm.process_event_current(event, elapsed);

        switch (this->sm.change_view(new_view))
        {
        case -1:
            // an error occured
            std::cout << "Unable to find the view " << new_view << " to process the events" << std::endl;
            break;

        case 0:
            break;

        case 1:
            std::cout << "Changed view (Game::dispatch_events). Old one was " << c_view << std::endl;
            break;
        }
    }
    else
    {
        // error
        std::cout << "Unable to find the view " << c_view << " to process the events" << std::endl;
    }
}

void Game::render()
{
    int c_view = this->sm.getId();
    // launch the scripts
    PyScripting::run_drawing_modules();

    if (c_view != -1) // does the view exist ?
    {
        this->sm.render_current(this->window);
    }
    else
    {
        // error
        std::cout << "Unable to find the view " << c_view << " to render it" << std::endl;
    }
}

void Game::render_loading()
{
    this->window.draw(this->shape);
    this->window.draw(this->loading_text);
}

void Game::update(sf::Time elapsed)
{
    int c_view = this->sm.getId();
    // launch the scripts
    PyScripting::run_update_modules();

    if (c_view != -1) // does the view exist ?
    {
        this->sm.update_current(this->window, elapsed);
    }
    else
    {
        // error
        std::cout << "Unable to find the view " << c_view << " to update it" << std::endl;
    }
}

void Game::update_loading(sf::Time elapsed)
{
    if (this->shape_increasing)
        this->shape_outline_sickness++;
    else
        this->shape_outline_sickness--;

    this->shape.setOutlineThickness(this->shape_outline_sickness);
    this->shape.setRadius(50.0f - this->shape_outline_sickness);
    this->shape.setOrigin(
                          this->shape.getRadius() / 2 - this->shape_outline_sickness / 2
                          , this->shape.getRadius() / 2 - this->shape_outline_sickness / 2);

    if (this->shape_increasing && this->shape_outline_sickness == 48)
        this->shape_increasing = false;
    else if (!this->shape_increasing && this->shape_outline_sickness == 2)
        this->shape_increasing = true;
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

void Game::loading()
{
    int _fps_update = 0;
    sf::Time elapsed;

    bool load_sm = false;
    bool load_musics = false;

    sf::Event event;
    while (this->window.isOpen())
    {
        // load a new sprite
        if (!load_sm)
        {
            if(this->crea_load.load_next())
                load_sm = true;
        }
        if (load_sm && !load_musics)
        {
            if (this->sm.load())
                load_musics = true;
        }
        if (load_musics)
        {
            if (this->mplayer.load())
                break;
        }

        // get deltatime
        sf::Time dt = this->clock.restart();

        // update FPS and display
        this->update_fps(dt, _fps_update);
        elapsed += dt;
        this->update_loading(elapsed);

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
        }

        // rendering
        this->window.clear();
        this->render_loading();
        this->window.display();
    }
}

// public
Game::Game() :
    window(sf::VideoMode(WIN_W, WIN_H), WIN_TITLE, sf::Style::Titlebar | sf::Style::Close)
    , crea_load()
    , shape(50)
    , shape_outline_sickness(10)
    , shape_increasing(true)
{
    // creating base folders
    system("mkdir saves");
    system("mkdir screenshots");

    // load scripting module
    PyScripting::connect();
    PyScripting::setValue(10);

    // shapes
    this->shape.setFillColor(sf::Color(150, 50, 250));
    this->shape.setOutlineThickness(10);
    this->shape.setOutlineColor(sf::Color(250, 150, 50));
    this->shape.setOrigin(25.0f, 25.0f);
    this->shape.setPosition(int(WIN_W / 2 - 25), int(WIN_H / 2 - 25));

    // font
    if (!this->font.loadFromFile("assets/fonts/pkmnemn.ttf"))
        std::cout << "Missing font at assets/fonts/pkmnemn.ttf" << std::endl;

    // texts
    this->loading_text.setFont(this->font);
    this->loading_text.setString("Chargement ...");
    this->loading_text.setCharacterSize(24);
    this->loading_text.setPosition(5.0f, 5.0f);

    ObjectsTable::load();
}

int Game::run()
{
    this->loading();
    // we "add" a default view
    this->sm.change_view(DEFAULT_VIEW_ID);
    this->sm.getInventory()->add_bag(this->sm.getDefault()->getCharacter()->getBag());
    // launch the scripts
    PyScripting::run_on_start_modules();

    int _fps_update = 0;

    // musics
    this->mplayer.play(this->mplayer.getNext());

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

    // launch the scripts
    PyScripting::run_on_end_modules();

    return 0;
}
