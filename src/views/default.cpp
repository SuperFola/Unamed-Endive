#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "../../debug.hpp"

#include "default.hpp"
#include "../scripting/scripting.hpp"

std::vector<std::string> glob_frag(const std::string& directory)
{
    std::vector<std::string> files;

    #ifdef PLATFORM_WIN
    WIN32_FIND_DATA File;
    HANDLE hSearch;

    hSearch = FindFirstFile((directory + "/*.frag").data(), &File);
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
            std::string t = std::string(ent->d_name);
            if (t.size() > 4 && t.substr(t.size() - 4, t.size()) == "frag")
                files.push_back(ent->d_name);
        }

        closedir(rep);
    }
    #endif // PLATFORM_POSIX

    return files;
}

void DefaultView::set_view(sf::RenderWindow& window)
{
    this->view.setCenter(this->player.getPos().getX() + TILE_SIZE, this->player.getPos().getY() + TILE_SIZE);
    window.setView(this->view);
}

void DefaultView::unset_view(sf::RenderWindow& window)
{
    window.setView(window.getDefaultView());
}

DefaultView::DefaultView() :
    View(DEFAULT_VIEW_ID)
    , view(sf::FloatRect(0, 0, WIN_W, WIN_H))
    , level("assets/map/0.umd")         // default map
    , display_mmap(false)
    , _speaking_to_pnj(false, -1, -1)
    , current_shader("")
{
}

bool DefaultView::load() { return true; }

bool DefaultView::load(sf::String folder)
{
    // loading shaders
    if (sf::Shader::isAvailable())
    {
        for (auto& frag : glob_frag("assets/shaders"))
            this->shaders.push_back(frag);
    }
    if (!this->distortionMap.loadFromFile("assets/shaders/textures/noiseWater.png"))
        return false;

    // loading game data
    if (is_file_existing("saves/" + folder.toAnsiString() + "/map.json"))
    {
        std::ifstream file("saves/" + folder.toAnsiString() + "/map.json");
        Json::Value root;
        file >> root;
        this->level.setMapPath("assets/map/" + to_string<int>(root["id"].asInt()) + ".umd");
    }
    this->level.load_map_at();  // empty will cause to load the map given by default

    // loading UI
    if (!this->menu_hud.load())
    {
        DebugLog(SH_ERR, "An error occured while loading the menu");
        return false;
    }

    // configuring stuff
    this->player.setFolder(folder.toAnsiString());
    if (!this->player.load())
    {
        DebugLog(SH_ERR, "An error occured while loading the player");
        return false;
    }
    this->player.setPNJm(&this->pnjmgr);

    if (!this->offscreen.create(WIN_W, WIN_H))
        return false;
    if (!this->minimap.create(MINIMAP_X, MINIMAP_Y))
        return false;
    if (!this->world.create(WIN_W, WIN_H))
        return false;

    this->offsprite.setTexture(this->offscreen.getTexture());
    this->minisprite.setTexture(this->minimap.getTexture());
    this->worldsprite.setTexture(this->world.getTexture());

    return true;
}

void DefaultView::render(sf::RenderWindow& window)
{
    // setting the views
    if (!this->level.smaller_than_window())
        this->set_view(window);
    else
    {
        this->view.setCenter(this->level.getWidth() / 2 * TILE_SIZE, this->level.getHeight() / 2 * TILE_SIZE);
        window.setView(this->view);
    }

    sf::View mview = sf::View(sf::FloatRect(0 , 0 , MINIMAP_X,  MINIMAP_Y));
    mview.setCenter(this->level.getWidth() / 2 * TILE_SIZE, this->level.getHeight() / 2 * TILE_SIZE);
    mview.zoom(0.5f * float(MINIMAP_X) / float(this->level.getWidth()));
    this->minimap.setView(mview);

    // rendering on RenderTextures
    this->menu_hud.render(this->offscreen);
    this->level.micro_render(this->minimap);
    this->minimap.display();

    // normal rendering (level and chara)
    this->level.render(this->world);
    bool rendered_chara = false;

    // pnj rendering
    int mid = this->level.getId();
    for (int i=0; i < this->pnjmgr.countPNJonMap(this->level.getId()); i++)
    {
        this->world.draw(this->pnjmgr.getPNJonMap(mid, i).getCurrentSprite());
        this->pnjmgr.getPNJonMap(mid, i).render(this->offscreen);

        // we check if the player is right in front of a NPC
        bool condition = this->pnjmgr.getPNJonMap(mid, i).getPos().intersect(
                                this->player.getPos()
                                , int(this->pnjmgr.getPNJonMap(mid, i).getCurrentSprite().getGlobalBounds().width)
                                , int(this->pnjmgr.getPNJonMap(mid, i).getCurrentSprite().getGlobalBounds().height)
                         ) &&
                         this->pnjmgr.getPNJonMap(mid, i).getPos().getY() < this->player.getPos().getY()
            ;

        if (!rendered_chara && condition)
        {
            rendered_chara = true;
            this->world.draw(this->player.getCurrentSprite());
        }
    }
    if (!rendered_chara)
        this->world.draw(this->player.getCurrentSprite());
    this->level.render_top(this->world);

    // rendering with our shaders now !! :D
    this->world.display();
    if (this->current_shader != "")
        window.draw(this->worldsprite, &this->shader);
    else
        window.draw(this->worldsprite);

    // drawing stuff relative to the top left corner of the window, not the top left corner of the view ;)
    this->offscreen.display();
    sf::Vector2f p = window.mapPixelToCoords(sf::Vector2i(0, 0));
    this->offsprite.setPosition(p);
    window.draw(this->offsprite);

    // displaying (or not) the mini map
    if (this->display_mmap && !this->menu_hud.isTriggered())
    {
        sf::Vector2f p2 = window.mapPixelToCoords(sf::Vector2i(WIN_W - MINIMAP_X - 4, 4));
        this->minisprite.setPosition(p2);
        window.draw(this->minisprite);
    }
}

void DefaultView::update(sf::RenderWindow& window, sf::Time elapsed)
{
    this->player.update(window, elapsed);
    this->level.update(window, elapsed);
    this->pnjmgr.update(this->level.getId(), window, elapsed);
    this->menu_hud.update(window, elapsed);

    this->offscreen.clear(sf::Color::Transparent);
    this->minimap.clear(sf::Color::Transparent);

    if (OMessenger::get().target_view == this->getId())
    {
        if (OMessenger::get().action == ObjAction::nothing)
        {
            if (OMessenger::get().type == ObjType::player)
            {
                int s = OMessenger::get().value;

                if (this->player.getSpeed() == s)
                    this->player.resetSpeed();
                else
                    this->player.setSpeed(s);
            }
        }
        OMessenger::empty();
    }
}

int DefaultView::process_event(sf::Event&, sf::Time) {}

int DefaultView::process_event(sf::Event& event, sf::Time elapsed, sf::RenderWindow& window)
{
    bool has_triggered_hud = false;
    bool has_moved = false;
    std::string k = "";

    if (this->menu_hud.isTriggered())
        goto menu_hud_ev_processing;

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        this->resolve_pnjspeak(m__X, m__Y, window);
    }
    else if (event.type == sf::Event::TextEntered)
        k = SettingsView::convert_textentered_to_value(event.text.unicode);
    else if (event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::Space:
            has_moved = true;  // kind of tricky, just to stop speaking to the pnj
            PyScripting::run_code(("trigger_event(" +
                                   to_string<int>(this->level.getId()) + "," +
                                   to_string<int>(this->player.getPos().getX() / TILE_SIZE) + "," +
                                   to_string<int>(this->player.getPos().getY() / TILE_SIZE) + "," +
                                   "'action button')").c_str());
            break;

        default:
            k = SettingsView::convert_textentered_to_value(event.key.code, true);
            break;
        }
    }

    if (k != "")
    {
        if (Config::get("menu") == k)
        {
            this->menu_hud.setTrigger(true);
            has_triggered_hud = true;
        }
        else if (Config::get("up") == k)
        {
            this->player.move(DIRECTION::up, this->level, elapsed);
            has_moved = true;
        }
        else if (Config::get("down") == k)
        {
            this->player.move(DIRECTION::down, this->level, elapsed);
            has_moved = true;
        }
        else if (Config::get("left") == k)
        {
            this->player.move(DIRECTION::left, this->level, elapsed);
            has_moved = true;
        }
        else if (Config::get("right") == k)
        {
            this->player.move(DIRECTION::right, this->level, elapsed);
            has_moved = true;
        }
    }

    if (has_moved || has_triggered_hud)
        this->disable_pnj_speaking();

    menu_hud_ev_processing:
    if (!has_triggered_hud) // if we triggered the hud, sending the event to it will cause to close it immediately
        return this->menu_hud.process_event(event, elapsed);
    return -1;  // we didn't triggered the hud so we don't need to change the view
}

bool DefaultView::hasActiveHud()
{
    return this->menu_hud.isTriggered();
}

Character* DefaultView::getCharacter()
{
    return &this->player;
}

PNJManager* DefaultView::getPNJM()
{
    return &this->pnjmgr;
}

Map* DefaultView::getMap()
{
    return &this->level;
}

void DefaultView::change_display_mmap(bool value)
{
    this->display_mmap = value;
}

bool DefaultView::get_display_mmap()
{
    return this->display_mmap;
}

void DefaultView::draw_on_offscreen(const sf::Drawable& drawable)
{
    this->offscreen.draw(drawable);
}

void DefaultView::resolve_pnjspeak(int x, int y, sf::RenderWindow& window)
{
    int mid = this->level.getId();
    sf::Vector2f pixPos = window.mapPixelToCoords(sf::Vector2i(x, y), this->view);
    int pnji = this->pnjmgr.find_pnjid_at(int(pixPos.x), int(pixPos.y), mid);

    if (pnji != -1)
    {
        Point _pos = this->pnjmgr.getPNJonMap(mid, pnji).getPos();
        if (this->player.getPos().squareDistanceTo(_pos) <= MAX_DIST_FROM_PNJ_IN_CASE * MAX_DIST_FROM_PNJ_IN_CASE * TILE_SIZE * TILE_SIZE)
        {
            this->pnjmgr.getPNJonMap(mid, pnji).speak();
            this->_speaking_to_pnj = std::make_tuple(true, mid, pnji);
        }
    }
}

void DefaultView::disable_pnj_speaking()
{
    if (std::get<0>(this->_speaking_to_pnj))
    {
        // we are speaking to a pnj
        this->pnjmgr.getPNJonMap(std::get<1>(this->_speaking_to_pnj), std::get<2>(this->_speaking_to_pnj)).speak();

        this->_speaking_to_pnj = std::make_tuple(false, -1, -1);
    }
}

void DefaultView::setShader(const std::string& name)
{
    if ((name == "" || std::find(this->shaders.begin(), this->shaders.end(), name) != this->shaders.end()) && sf::Shader::isAvailable())
    {
        this->current_shader = name;
        if (!this->shader.loadFromFile(this->shaders_path + name, sf::Shader::Fragment))
            DebugLog(SH_ERR, "Could not load shader at " << this->shaders_path << name);
        else
        {
            this->shader.setUniform("texture", this->world.getTexture());
            this->shader.setUniform("distorsionMapTexture", this->distortionMap);
            this->shader.setUniform("width", float(WIN_W));
            this->shader.setUniform("sigma", 0.5f);
        }
    }
}

const std::string& DefaultView::getCurrentShader()
{
    return this->current_shader;
}
