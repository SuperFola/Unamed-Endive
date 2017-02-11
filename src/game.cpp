#include <iostream>
#include <ctime>
#include <string>

#include "game.hpp"

// private
void Game::dispatch_events(sf::Event& event, sf::Time elapsed)
{
    int c_view = this->sm.getId();
    // we give the current event to the scripting engine if a script need it
    if (!this->cheat_on)
        PyScripting::setEvent(event);
    PyScripting::run_event_modules();

    if (c_view != -1)  // we check if the view exist
    {
        int new_view = UNREACHABLE_VIEW_ID;
        if (!this->cheat_on)
            new_view = this->sm.process_event_current(event, elapsed);

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

    if (this->cheat_on && c_view == DEFAULT_VIEW_ID)
        this->sm.getDefault()->draw_on_offscreen(this->cmd);

    if (c_view != -1) // does the view exist ?
    {
        this->sm.render_current(this->window);
    }
    else
    {
        // error
        std::cout << "Unable to find the view " << c_view << " to render it" << std::endl;
    }
    // launch the scripts
    PyScripting::run_drawing_modules();
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

void Game::take_screenshot()
{
    time_t t = time(0);
    struct tm* now = localtime(&t);
    sf::Vector2u windowSize = this->window.getSize();
    sf::Texture texture;

    texture.create(windowSize.x, windowSize.y);
    texture.update(this->window);

    sf::Image screenshot = texture.copyToImage();
    screenshot.saveToFile(
                          std::string("screenshots/screenshot-")
                          + to_string<int>(now->tm_year + 1900)
                          + std::string("-")
                          + to_string<int>(now->tm_mon + 1)
                          + std::string("-")
                          + to_string<int>(now->tm_mday)
                          + std::string(" - ")
                          + to_string<int>(now->tm_hour)
                          + std::string("-")
                          + to_string<int>(now->tm_min)
                          + std::string("-")
                          + to_string<int>(now->tm_sec)
                          + std::string(".jpg")
                        );
}

// public
Game::Game() :
    window(sf::VideoMode(WIN_W, WIN_H), WIN_TITLE, sf::Style::Titlebar | sf::Style::Close)
    , crea_load()
    , shape(50)
    , shape_outline_sickness(10)
    , shape_increasing(true)
    , cheat_on(false)
    , _got_coderet(false)
{
    // creating base folders
    system("mkdir saves");
    system("mkdir screenshots");

    // scripting module
    PyScripting::connect();

    // window
    this->window.setFramerateLimit(FRAMERATE_LIM);
    this->window.setTitle(WIN_TITLE);
    sf::Image ico;
    ico.loadFromFile("assets/unamedlogo.png");
    this->window.setIcon(ICO_X, ICO_Y, ico.getPixelsPtr());

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

    this->cmd.setFont(this->font);
    this->cmd.setCharacterSize(24);
    this->cmd.setPosition(10.0f, 10.0f);
    this->cmd.setColor(sf::Color::White);

    ObjectsTable::load();
    this->ttable.load();
}

void Game::post_load()
{
    // views
    // we "add" a default view
    this->sm.change_view(DEFAULT_VIEW_ID);
    // we add the bag to the inventory view from the player
    this->sm.getInventory()->add_bag(this->sm.getDefault()->getCharacter()->getBag());
    // we add the equip to the creatures view from the player
    this->sm.getCrea()->add_equip(this->sm.getDefault()->getCharacter()->getEquip());
    // we add the dex to the dex view from the player
    this->sm.getDex()->add_dex(this->sm.getDefault()->getCharacter()->getDex());
    // we add a pointer on the creatures loader to the dex /!/
    this->sm.getDex()->add_crealoader(&this->crea_load);

    // scripting
    PyScripting::setWindow(&this->window);
    PyScripting::setMusicPlayer(&this->mplayer);
    PyScripting::setStateMachine(&this->sm);
    // we add the pnj manager here (very important, otherwise it won't be loaded in the default if we try to add it in the cstr of the class) to the scripting engine
    PyScripting::setPnjManager(this->sm.getDefault()->getPNJM());
    // same here
    PyScripting::setMap(this->sm.getDefault()->getMap());
    PyScripting::setTriggsMgr(this->sm.getDefault()->getTriggsMgr());
    PyScripting::setPlayer(this->sm.getDefault()->getCharacter());
    // load them all (the scripts)
    PyScripting::load();
    // launch the scripts
    PyScripting::run_on_start_modules();
}

void Game::on_end()
{
    // launch the scripts
    PyScripting::run_on_end_modules();

    // testing
    this->sm.getDefault()->getCharacter()->save();
}

int Game::run()
{
    this->loading();
    this->post_load();

    int _fps_update = 0;

    // musics
    #ifndef DEV_MODE
    this->mplayer.play(this->mplayer.getNext());
    #endif

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

            #ifdef DEV_MODE
            case sf::Event::TextEntered:
                if (this->cheat_on)
                {
                    if (event.text.unicode == '\b' && this->cmd_str.getSize() > 0)
                    {
                        this->cmd_str.erase(this->cmd_str.getSize() - 1, 1);
                    }
                    else if (event.text.unicode == 13)  // return
                    {
                        if (!this->_got_coderet)
                        {
                            this->cmd_str = sf::String(PyScripting::run_code_and_get_out(this->cmd_str.toAnsiString().c_str()));
                            this->_got_coderet = true;
                        }
                        else
                        {
                            this->cmd_str.clear();
                            this->_got_coderet = false;
                        }
                    }
                    else
                    {
                        if (this->_got_coderet)
                        {
                            this->cmd_str.clear();
                            this->_got_coderet = false;
                        }
                        this->cmd_str.insert(this->cmd_str.getSize(), event.text.unicode);
                    }
                    this->cmd.setString(this->cmd_str);
                }
                break;
            #endif // DEV_MODE

            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::F5:
                    // take screenshoot
                    this->take_screenshot();
                    break;

                case sf::Keyboard::F6:
                    // shut the music (or unshut it if it has already been used)
                    if (this->mplayer.getState())
                        this->mplayer.stop();
                    else
                        this->mplayer.play(this->mplayer.getCurrentName());
                    break;

                case sf::Keyboard::M:
                    // display or not the mini map
                    if (!this->cheat_on)
                        this->sm.getDefault()->change_display_mmap(!this->sm.getDefault()->get_display_mmap());
                    break;

                #ifdef DEV_MODE
                case sf::Keyboard::C:
                    std::cout << "test" << std::endl;
                    this->sm.getDefault()->getCharacter()->getEquip()->getCrea(0)->print();
                    break;

                case sf::Keyboard::F8:
                    std::cout << "Reloading scripts" << std::endl;
                    PyScripting::reload_all();
                    break;

                case sf::Keyboard::F4:
                    this->cheat_on = !this->cheat_on;
                    if (this->cheat_on)
                    {
                        std::cout << "Cheats on" << std::endl;
                        this->cmd.setPosition(10.0f, 10.0f);
                    }
                    else
                    {
                        std::cout << "Cheats off" << std::endl;
                    }
                    break;
                #endif

                default:
                    break;
                }
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

    this->on_end();

    return 0;
}
