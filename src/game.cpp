#include <iostream>
#include <ctime>
#include <string>
#include <cstdlib>

#include "../debug.hpp"

#include "../version.h"

#include "game.hpp"

std::vector<std::string> glob(const std::string& directory)
{
    std::vector<std::string> files;

    #ifdef PLATFORM_WIN
    WIN32_FIND_DATA File;
    HANDLE hSearch;

    hSearch = FindFirstFile((directory + "/*").data(), &File);
    if (hSearch != INVALID_HANDLE_VALUE)
    {
        do {
                if (std::string(File.cFileName) != "." && std::string(File.cFileName) != "..")
                    files.push_back(std::string(File.cFileName));
        } while (FindNextFile(hSearch, &File));

        FindClose(hSearch);
    }
    #endif // PLATFORM_WIN

    #ifdef PLATFORM_POSIX
    DIR* rep = opendir(directory.data());

    if (rep != NULL)
    {
        struct dirent* ent;

        while ((ent = readdir(rep)) != NULL)
        {
            files.push_back(ent->d_name);
        }

        closedir(rep);
    }
    #endif // PLATFORM_POSIX

    return files;
}

// private
void Game::handle_std_events(sf::Event& event, sf::Time elapsed)
{
    switch (event.type)
    {
        case sf::Event::TextEntered:
            if (this->inner_balloon_prompt_triggered == 1)
            {
                if (event.text.unicode == '\b' && this->inner_balloon_prompt_str.getSize() > 0)
                    this->inner_balloon_prompt_str.erase(this->inner_balloon_prompt_str.getSize() - 1, 1);
                else if (event.text.unicode == 13)  // validate
                    this->inner_balloon_prompt_triggered = 2;
                else if (this->inner_balloon_prompt_str.getSize() < this->inner_balloon_prompt_max_length)
                    this->inner_balloon_prompt_str.insert(this->inner_balloon_prompt_str.getSize(), event.text.unicode);
            }
            #ifdef DEV_MODE
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
            #endif // DEV_MODE
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
            case sf::Keyboard::F5:
                // take screenshot
                this->take_screenshot();
                break;

            case sf::Keyboard::F6:
                // mute the music
                if (this->mplayer.getState())
                {
                    this->mplayer.mute();

                    if (this->mplayer.getVolume() == 0.0f)
                        DebugLog(SH_INFO, "Music muted");
                    else
                        DebugLog(SH_INFO, "Music unmuted");
                }
                break;

            case sf::Keyboard::F10:
                // shut the music (or unshut it if it has already been used)
                if (this->mplayer.getState())
                {
                    this->mplayer.stop();
                    DebugLog(SH_INFO, "Music stopped");
                }
                else
                {
                    if (this->mplayer.play((this->mplayer.getCurrentName() != "") ? this->mplayer.getCurrentName() : this->mplayer.getNext()))
                        DebugLog(SH_INFO, "Music started");
                    else
                        DebugLog(SH_ERR, "Could not start music !");
                }
                break;

            case sf::Keyboard::M:
                // display or not the mini map
                if (!this->cheat_on)
                {
                    this->sm.getDefault()->change_display_mmap(!this->sm.getDefault()->get_display_mmap());
                    DebugLog(SH_INFO, "Display minimap : " << (this->sm.getDefault()->get_display_mmap() ? "true" : "false"));
                }
                break;

            #ifdef DEV_MODE
            case sf::Keyboard::F:
                if (!this->cheat_on)
                    this->sm.change_view(FIGHT_VIEW_ID);
                break;

            case sf::Keyboard::F8:
                DebugLog(SH_WARN, "Reloading scripts");
                PyScripting::reload_all();
                break;

            case sf::Keyboard::F4:
                this->cheat_on = !this->cheat_on;
                if (this->cheat_on)
                {
                    DebugLog(SH_SPE, "Cheats on");
                    this->cmd.setPosition(10.0f, 10.0f);
                }
                else
                {
                    DebugLog(SH_SPE, "Cheats off");
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
}

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
        /// if cheats on OR balloon prompt showed, do NOT dispatch events !
        if (!this->cheat_on && this->inner_balloon_prompt_triggered == 0)
            new_view = this->sm.process_event_current(event, elapsed, this->window);

        switch (this->sm.change_view(new_view))
        {
        case -1:
            // an error occured
            DebugLog(SH_ERR, "Unable to find the view " << new_view << " to process the events");
            break;

        case 0:
            break;

        case 1:
            DebugLog(SH_OK, "Changed view (Game::dispatch_events). Old one was " << c_view);
            break;
        }
    }
    else
    {
        // error
        DebugLog(SH_ERR, "Unable to find the view " << c_view << " to process the events");
    }
}

void Game::render()
{
    int c_view = this->sm.getId();

    if (this->inner_balloon_prompt_triggered == 1)
    {
        if (this->sm.getId() == DEFAULT_VIEW_ID)
        {
            this->sm.getDefault()->draw_on_offscreen(this->inner_balloon_prompt_sprite);
            this->sm.getDefault()->draw_on_offscreen(this->inner_balloon_prompt_txt);
        }
        else
        {
            window.draw(this->inner_balloon_prompt_sprite);
            window.draw(this->inner_balloon_prompt_txt);
        }
    }

    if (this->cheat_on && c_view == DEFAULT_VIEW_ID)
    {
        this->sm.getDefault()->draw_on_offscreen(this->cmd);
        if (this->blink)
            this->sm.getDefault()->draw_on_offscreen(this->cursor);
    }

    if (c_view != -1) // does the view exist ?
    {
        this->sm.render_current(this->window);
    }
    else
    {
        // error
        DebugLog(SH_ERR, "Unable to find the view " << c_view << " to render it");
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
        DebugLog(SH_ERR, "Unable to find the view " << c_view << " to update it");
    }

    // update blinking thing globally because as we say in french "flemme de le faire plusieurs fois"
    this->blinking++;
    if (!(this->blinking % 60))
    {
        this->blink = !this->blink;
        this->blinking %= 60;
    }

    if (this->cheat_on && c_view == DEFAULT_VIEW_ID)
    {
        this->cursor.setPosition(this->cmd.getPosition().x + this->cmd.getGlobalBounds().width + 5.0f, this->cmd.getPosition().y);
    }

    // update inner_balloon_prompt_txt
    if (this->inner_balloon_prompt_triggered == 1)
        this->inner_balloon_prompt_txt.setString(wrapText(this->inner_balloon_prompt_str_back + this->inner_balloon_prompt_str, 600, this->font, this->inner_balloon_prompt_txt.getCharacterSize(), false));

    // add a blinking "_" at the end of the inner_balloon_prompt_txt only if there isn't any and if it is active
    if (this->inner_balloon_prompt_triggered == 1 && this->blink &&
                    this->inner_balloon_prompt_txt.getString().substring(this->inner_balloon_prompt_txt.getString().getSize() - 1, 1) != sf::String("_"))
        this->inner_balloon_prompt_txt.setString(this->inner_balloon_prompt_txt.getString() + sf::String("_"));
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
            if (this->sm.load(this->menu_userentry))
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
                this->has_requested_quit = true;
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

void Game::update_menu(sf::Time elapsed, int s, bool new_game)
{
    // centering text
    if (new_game)
    {
        this->menu_user.setPosition(WIN_W / 2.0f - this->menu_user.getGlobalBounds().width / 2.0f, this->menu_user.getPosition().y);
    }
    this->blinking++;

    if (!(this->blinking % 60))
    {
        this->blink = !this->blink;
        this->blinking %= 60;
    }
}

void Game::render_menu(const std::vector<std::string>& s, bool new_game, bool del_game, bool play_game, bool valid_delete)
{
    // background + logo + version number
    this->window.draw(this->menu_bckg_s);
    this->window.draw(this->menu_logo_s);
    this->window.draw(this->ver);

    // alphablack if we are doing anything
    if (new_game || del_game || play_game)
        this->window.draw(this->alphablack);

    if (del_game || play_game)
    {
        // warning
        if (del_game)
            this->window.draw(this->deletewarn);
        if (valid_delete)
            this->window.draw(this->validatebtn);

        // drawing game choices
        int i = 0;
        for (const auto& elem : s)
        {
            this->menu_text.setString(elem);
            this->menu_text.setPosition(this->menu_text.getPosition().x, 250.0f + i * (this->menu_text.getCharacterSize() + 4.0f));

            if (i == this->menu_game_selected)
                this->menu_text.setFillColor(sf::Color::Green);
            else
                this->menu_text.setFillColor(sf::Color::White);

            this->window.draw(this->menu_text);

            i++;
        }
    }

    // buttons
    if (!new_game && !del_game && !play_game)
    {
        this->window.draw(this->menu_btn_start_s);
        this->window.draw(this->menu_btn_new_s);
        this->window.draw(this->menu_btn_del_s);
    }

    // draw text asking to the user its name
    if (new_game)
    {
        this->window.draw(this->menu_ask_user);
        this->window.draw(this->menu_user);

        if (this->blink)
        {
            this->cursor.setPosition(this->menu_user.getPosition().x + this->menu_user.getGlobalBounds().width + 10.0f, this->cursor.getPosition().y);
            this->window.draw(this->cursor);
        }
    }
}

void Game::menu()
{
    int _fps_update = 0;
    sf::Time elapsed;
    bool quit = false;
    bool new_game = false;
    bool del_game = false;
    bool play_game = false;
    bool delete_selected_game = false;

    std::vector<std::string> saves = glob("saves/");

    sf::Event event;
    while (this->window.isOpen())
    {
        // get deltatime
        sf::Time dt = this->clock.restart();

        // update FPS and display
        this->update_fps(dt, _fps_update);
        elapsed += dt;
        this->update_menu(elapsed, saves.size(), new_game);

        // dispatch events using a loop
        while (this->window.pollEvent(event))
        {
            // default events
            switch(event.type)
            {
            case sf::Event::KeyPressed:
                if (play_game || del_game || new_game)
                {
                    switch(event.key.code)
                    {
                    case sf::Keyboard::Up:
                        if (saves.size() > 0)
                        {
                            this->menu_game_selected--;
                            if (this->menu_game_selected < 0)
                                this->menu_game_selected = saves.size() - 1;
                        }
                        break;

                    case sf::Keyboard::Down:
                        if (saves.size() > 0)
                        {
                            this->menu_game_selected++;
                            if (this->menu_game_selected > saves.size() - 1)
                                this->menu_game_selected = 0;
                        }
                        break;

                    case sf::Keyboard::Escape:
                        new_game = false;
                        del_game = false;
                        play_game = false;
                        this->menu_userentry.clear();
                        this->menu_user.setString(this->menu_userentry);
                        this->menu_game_selected = -1;
                        break;

                    default:
                        break;
                    }
                }
                if (del_game && delete_selected_game)
                {
                    if (event.key.code == sf::Keyboard::Return)
                    {
                        pop<std::string>(&saves, this->menu_game_selected);
                        PyScripting::run_code((std::string("remove(\"saves/") + this->menu_userentry.toAnsiString() + "\")").c_str());
                        this->menu_userentry.clear();
                        this->menu_game_selected = -1;
                        delete_selected_game = false;
                    }
                }
                break;

            case sf::Event::TextEntered:
                if (new_game)
                {
                    if (event.text.unicode == '\b' && this->menu_userentry.getSize() > 0)
                        this->menu_userentry.erase(this->menu_userentry.getSize() - 1, 1);
                    else if (event.text.unicode == 13)  // validate
                        quit = true;
                    else if (this->menu_userentry.getSize() < 30)
                    {
                        this->menu_userentry.insert(this->menu_userentry.getSize(), event.text.unicode);
                    }
                    this->menu_user.setString(this->menu_userentry);
                }
                break;

            case sf::Event::MouseButtonPressed:
                switch(event.mouseButton.button)
                {
                case sf::Mouse::Button::Left:
                    // activate buttons only if not in a submenu
                    if (!play_game && !del_game && !new_game)
                    {
                        if (m__X >= this->menu_btn_new_s.getPosition().x && m__X <= this->menu_btn_new_s.getPosition().x + this->menu_btn_new_s.getGlobalBounds().width &&
                             m__Y >= this->menu_btn_new_s.getPosition().y && m__Y <= this->menu_btn_new_s.getPosition().y + this->menu_btn_new_s.getGlobalBounds().height)
                        {
                            // clic on button new
                            new_game = true;
                            this->menu_userentry.clear();
                        }
                        if (m__X >= this->menu_btn_del_s.getPosition().x && m__X <= this->menu_btn_del_s.getPosition().x + this->menu_btn_del_s.getGlobalBounds().width &&
                             m__Y >= this->menu_btn_del_s.getPosition().y && m__Y <= this->menu_btn_del_s.getPosition().y + this->menu_btn_del_s.getGlobalBounds().height)
                        {
                            // clic on button delete
                            del_game = true;
                        }
                        if (m__X >= this->menu_btn_start_s.getPosition().x && m__X <= this->menu_btn_start_s.getPosition().x + this->menu_btn_start_s.getGlobalBounds().width &&
                             m__Y >= this->menu_btn_start_s.getPosition().y && m__Y <= this->menu_btn_start_s.getPosition().y + this->menu_btn_start_s.getGlobalBounds().height)
                        {
                            // clic on button start
                            play_game = true;
                        }
                    }
                    else
                    {
                        if (m__X >= WIN_W / 2.0f - 200.0f && m__X <= WIN_W / 2.0f + 200.0f && m__Y >= 250.0f && m__Y <= 250.0f + saves.size() * (4.0f + this->menu_text.getCharacterSize()))
                        {
                            // clic on an existing save
                            int ry = (m__Y - 250) / (4.0f + this->menu_text.getCharacterSize());

                            if (play_game || del_game)
                            {
                                if (0 <= ry && ry <= saves.size() - 1)
                                {
                                    this->menu_userentry = saves[ry];
                                    this->menu_game_selected = ry;
                                }

                                if (del_game)
                                {
                                    delete_selected_game = true;
                                }
                                else if (play_game)
                                {
                                    quit = true;
                                }
                            }
                        }
                    }
                    break;

                default:
                    break;
                }
                break;

            case sf::Event::Closed:
                this->window.close();
                this->has_requested_quit = true;
                break;

            default:
                break;
            }
        }

        // rendering
        this->window.clear();
        this->render_menu(saves, new_game, del_game, play_game, delete_selected_game);
        this->window.display();

        if (quit)
            break;
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
    , has_requested_quit(false)
    , network(false)  /** Testing */
    , nethost("localhost")
    , netport(9999) // default port
    , netprotoc(Network::Protoc::TCP) // default communication protocole
    , shape(50)
    , shape_outline_sickness(10)
    , shape_increasing(true)
    , cheat_on(false)
    , _got_coderet(false)
    , menu_game_selected(-1)
    , inner_balloon_prompt_triggered(0)
    , inner_balloon_prompt_max_length(0)
{
    DebugLog(SH_INFO, "Running on " << AutoVersion::FULLVERSION_STRING);

    // configuration file
    Config::load();
    // loading types tables (percentage of +/- by type1 on type2)
    TypesTable::load();
    // loading objects table
    ObjectsTable::load();

    // OMessenger
    OMessenger::empty();

    // scripting module
    PyScripting::connect();

    // creating base folders
    PyScripting::run_code("if not os.path.exists(\"saves\"): os.mkdir(\"saves\")\nif not os.path.exists(\"screenshots\"): os.mkdir(\"screenshots\")");

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

    // images
      // textures
    sf::Texture menu_bckg;
    menu_bckg.loadFromFile("assets/menu/background.png");
    sf::Texture menu_logo;
    menu_logo.loadFromFile("assets/menu/logo_alpha.png");
    sf::Texture btn_del;
    btn_del.loadFromFile("assets/menu/btn_delete_game.png");
    sf::Texture btn_new;
    btn_new.loadFromFile("assets/menu/btn_new_game.png");
    sf::Texture btn_start;
    btn_start.loadFromFile("assets/menu/btn_start_game.png");
    sf::Texture _alphablack;
    _alphablack.loadFromFile("assets/menu/alphablack.png");
    sf::Texture delwarn;
    delwarn.loadFromFile("assets/menu/deletewarning.png");
    sf::Texture validate;
    validate.loadFromFile("assets/menu/validate.png");
    sf::Texture inner_balloon_prompt_tex;
    inner_balloon_prompt_tex.loadFromFile("assets/gui/pnj/bubble.png");
      // saving them
    this->textures.add("bckg", menu_bckg);
    this->textures.add("logo", menu_logo);
    this->textures.add("btn_new", btn_new);
    this->textures.add("btn_del", btn_del);
    this->textures.add("btn_start", btn_start);
    this->textures.add("alphablack", _alphablack);
    this->textures.add("delwarn", delwarn);
    this->textures.add("validate", validate);
    this->textures.add("inner_balloon_prompt", inner_balloon_prompt_tex);
      // creating the sprites
    this->menu_bckg_s.setTexture(this->textures.get("bckg"));
    this->menu_bckg_s.setPosition(0.0f, 0.0f);
    this->menu_logo_s.setTexture(this->textures.get("logo"));
    this->menu_logo_s.setPosition(WIN_W / 2.0f - this->textures.get("logo").getSize().x / 2.0f, 20.0f);
    this->menu_btn_new_s.setTexture(this->textures.get("btn_new"));
    this->menu_btn_new_s.setPosition(WIN_W / 2.0f - this->textures.get("btn_new").getSize().x / 2.0f, WIN_H / 2 - (this->textures.get("btn_new").getSize().y + 20.0f));
    this->menu_btn_start_s.setTexture(this->textures.get("btn_start"));
    this->menu_btn_start_s.setPosition(WIN_W / 2.0f - this->textures.get("btn_start").getSize().x / 2.0f, WIN_H / 2);
    this->menu_btn_del_s.setTexture(this->textures.get("btn_del"));
    this->menu_btn_del_s.setPosition(WIN_W / 2.0f - this->textures.get("btn_del").getSize().x / 2.0f, WIN_H / 2 + (this->textures.get("btn_del").getSize().y + 20.0f));
    this->alphablack.setTexture(this->textures.get("alphablack"));
    this->alphablack.setPosition(0.0f, 0.0f);
    this->deletewarn.setTexture(this->textures.get("delwarn"));
    this->deletewarn.setPosition(0.0f, 0.0f);
    this->validatebtn.setTexture(this->textures.get("validate"));
    this->validatebtn.setPosition(0.0f, 0.0f);
    this->inner_balloon_prompt_sprite.setTexture(this->textures.get("inner_balloon_prompt"));
    this->inner_balloon_prompt_sprite.setPosition(10.0f, WIN_H - 110.0f);

    // font
    if (!this->font.loadFromFile(FONTPATH))
        DebugLog(SH_ERR, "Missing font at " << FONTPATH);

    // texts
    setupFont(this->loading_text, this->font, sf::Color::White, 24)
    this->loading_text.setString("Chargement ...");
    this->loading_text.setPosition(5.0f, 5.0f);

    setupFont(this->cmd, this->font, sf::Color::White, 24)
    this->cmd.setPosition(10.0f, 10.0f);

    setupFont(this->menu_user, this->font, sf::Color::White, 24)
    this->menu_user.setPosition(0.0f, WIN_H / 2.0f - 72.0f);

    setupFont(this->menu_text, this->font, sf::Color::White, 24)
    this->menu_text.setPosition(WIN_W / 2.0f - 100.0f, 0.0f);

    setupFont(this->menu_ask_user, this->font, sf::Color::White, 24)
    this->menu_ask_user.setString("Nom de la sauvegarde : ");
    this->menu_ask_user.setPosition(WIN_W / 2.0f - this->menu_ask_user.getGlobalBounds().width / 2.0f, WIN_H / 2.0f - 100.0f);

    setupFont(this->cursor, this->font, sf::Color::White, 24)
    this->cursor.setString("_");
    this->cursor.setPosition(0.0f, WIN_H / 2.0f - 72.0f);

    setupFont(this->ver, this->font, sf::Color::White, 20)
    this->ver.setString(std::string("v") + std::string(AutoVersion::FULLVERSION_STRING));
    this->ver.setPosition(WIN_W - 10.0f - this->ver.getGlobalBounds().width, WIN_H - 10.0f - this->ver.getGlobalBounds().height);

    setupFont(this->inner_balloon_prompt_txt, this->font, sf::Color::Black, 18)
    this->inner_balloon_prompt_txt.setPosition(25.0f, WIN_H - 100.0f);

    this->blink = false;
    this->blinking = 0;

    srand(static_cast<unsigned int>(time(0)));
}

void Game::post_load()
{
    DebugLog(SH_INFO, "Post load");

    // views
    // we "add" a default view
    this->sm.change_view(DEFAULT_VIEW_ID);
    // we add the bag to the inventory view from the player
    this->sm.getInventory()->add_bag(this->sm.getDefault()->getCharacter()->getBag());
    // we add the equip/pc/crealoader/dex to the creatures view from the player
    this->sm.getCrea()->add_equip(this->sm.getDefault()->getCharacter()->getEquip());
    this->sm.getCrea()->add_pc(this->sm.getDefault()->getCharacter()->getPC());
    this->sm.getCrea()->add_creaload(&this->crea_load);
    this->sm.getCrea()->add_dex(this->sm.getDefault()->getCharacter()->getDex());
    // we add the dex to the dex view from the player
    this->sm.getDex()->add_dex(this->sm.getDefault()->getCharacter()->getDex());
    // we add a pointer on the creatures loader to the dex .|^!^|.
    this->sm.getDex()->add_crealoader(&this->crea_load);
    // pointer to sm to save
    this->sm.getSave()->setdefv(this->sm.getDefault());
    // pointer on the dex to the fight view
    this->sm.getFight()->set_dex(this->sm.getDefault()->getCharacter()->getDex());
    this->sm.getFight()->set_equip(this->sm.getDefault()->getCharacter()->getEquip());
    this->sm.getFight()->set_pc(this->sm.getDefault()->getCharacter()->getPC());
    this->sm.getFight()->set_crealoader(&this->crea_load);
    // pointer on the music player for the settings view
    this->sm.getSettings()->set_music_player(&this->mplayer);

    // scripting
    PyScripting::setWindow(&this->window);
    PyScripting::setMusicPlayer(&this->mplayer);
    PyScripting::setStateMachine(&this->sm);
    // we add the pnj manager here (very important, otherwise it won't be loaded in the default if we try to add it in the cstr of the class) to the scripting engine
    PyScripting::setPnjManager(this->sm.getDefault()->getPNJM());
    // same here
    PyScripting::setMap(this->sm.getDefault()->getMap());
    PyScripting::setPlayer(this->sm.getDefault()->getCharacter());
    PyScripting::setGame(this);
    // load them all (the scripts)
    PyScripting::load();
    // launch the scripts
    PyScripting::run_on_start_modules();

    this->sm.getCrea()->post_load();
    this->sm.getDefault()->getCharacter()->getPC()->increase_size(MAX_SIZE_PC);

    this->sm.getDefault()->setShader(Config::get("shader").asString());

    #ifdef PLATFORM_WIN
        DebugLog(SH_INFO, "Platform: Windows");
    #endif // PLATFORM_WIN
    #ifdef PLATFORM_POSIX
        DebugLog(SH_INFO, "Platform: Posix");
    #endif // PLATFORM_POSIX

    if (this->network)
    {
        Network::Connection::init(this->nethost, this->netport, this->netprotoc);
        Network::Connection::start();
        Network::Connection::connect();
        // il y a longtemps : we skip Network::Connection::isSecured() and getInfo(), only useful when playing with other players, not in solo
        // 29/06 ah ok je viens de comprendre, je voulais mettre le jeu sur un serveur permanent mdr
        Network::Connection::auth(this->sm.getDefault()->getCharacter()->getName(), "");
    }
}

void Game::on_end()
{
    // launch the scripts
    PyScripting::run_on_end_modules();

    // saving, JIC
    this->sm.getSave()->save();

    Config::save();
}

void Game::trigger_inner_balloon_prompt(bool v)
{
    this->inner_balloon_prompt_triggered = v ? 1 : 0;
}

void Game::set_balloon_prompt(const char* text, int len)
{
    DebugLog(SH_INFO, "set balloon prompt : '" << text << "'");
    this->inner_balloon_prompt_txt.setString(std::string(text));
    this->inner_balloon_prompt_str_back = std::string(text);
    this->inner_balloon_prompt_max_length = len;
}

bool Game::get_triggered_inner_balloon_prompt()
{
    return inner_balloon_prompt_triggered >= 1;
}

const char* Game::get_inner_balloon_text()
{
    // == 2 means user has validate the entry
    if (this->inner_balloon_prompt_triggered == 2)
    {
        DebugLog(SH_INFO, "Returning value for balloon prompt");
        const char* s = this->inner_balloon_prompt_str.toAnsiString().c_str();
        // we put back the `sm` into its basic's state
        this->inner_balloon_prompt_triggered = 0;
        this->inner_balloon_prompt_str.clear();
        this->inner_balloon_prompt_str_back.clear();
        this->inner_balloon_prompt_txt.setString("");

        return s;
    }
    else
        return 0; // the text entry wasn't validated
}

int Game::run()
{
    this->menu();
    if (!this->has_requested_quit)
        this->loading();
    if (!this->has_requested_quit)
        this->post_load();

    int _fps_update = 0;
    sf::Event event;

    float timer = 1.0f;

    if (this->has_requested_quit)
        goto hell;

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
            this->handle_std_events(event, dt);
            this->dispatch_events(event, dt);
        }

        // rendering
        this->window.clear();
        this->render();
        this->window.display();

        // shaders things
        timer += 1.0f;
        this->sm.getDefault()->setShaderParameter("timer", timer);
    }

    this->on_end();

    hell:
    return 0;
}

void Game::quit()
{
    PyScripting::disconnect();
}
