#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "../../debug.hpp"

#include "default.hpp"

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
    , level("assets/map/0.umd")
    , display_mmap(false)
{
}

bool DefaultView::load() { return true; }

bool DefaultView::load(sf::String playername)
{
    if (is_file_existing("saves/" + playername.toAnsiString() + "/map.json"))
    {
        std::ifstream file("saves/" + playername.toAnsiString() + "/map.json");
        Json::Value root;
        file >> root;
        this->level.setMapPath("assets/map/" + to_string<int>(root["id"].asInt()) + ".umd");
    }

    this->level.load_map_at();  // empty will cause to load the map given by default

    if (!this->menu_hud.load())
    {
        DebugLog(SH_ERR, "An error occured while loading the menu");
        return false;
    }

    this->pnjmgr.add_pnj_on_map(this->level.getId(), "vader", "Je suis Vader, un commander d'El Padrino !", PNJkind::special, "Vader");

    this->player.setName(playername.toAnsiString());
    if (!this->player.load())
    {
        DebugLog(SH_ERR, "An error occured while loading the player");
        return false;
    }

    if (!this->offscreen.create(WIN_W, WIN_H))
        return false;
    if (!this->minimap.create(MINIMAP_X, MINIMAP_Y))
        return false;

    this->offsprite.setTexture(this->offscreen.getTexture());
    this->minisprite.setTexture(this->minimap.getTexture());

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
    this->level.render(window);
    window.draw(this->player.getCurrentSprite());

    // pnj rendering
    int mid = this->level.getId();
    for (int i=0; i < this->pnjmgr.countPNJonMap(this->level.getId()); i++)
    {
        window.draw(this->pnjmgr.getPNJonMap(mid, i).getCurrentSprite());
        this->pnjmgr.getPNJonMap(mid, i).render(this->offscreen);
    }
    this->level.render_top(window);

    this->offscreen.display();
    sf::Vector2f p = window.mapPixelToCoords(sf::Vector2i(0, 0));
    this->offsprite.setPosition(p);
    window.draw(this->offsprite);

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
}

int DefaultView::process_event(sf::Event& event, sf::Time elapsed)
{
    bool has_triggered_hud = false;

    if (this->menu_hud.isTriggered())
        goto menu_hud_ev_processing;

    switch(event.type)
    {
    case sf::Event::KeyPressed:
        switch(event.key.code)
        {
        case sf::Keyboard::E:
            this->menu_hud.setTrigger(true);
            has_triggered_hud = true;
            break;

        case sf::Keyboard::Z:
            this->player.move(DIRECTION::up, this->level, elapsed);
            break;

        case sf::Keyboard::S:
            this->player.move(DIRECTION::down, this->level, elapsed);
            break;

        case sf::Keyboard::Q:
            this->player.move(DIRECTION::left, this->level, elapsed);
            break;

        case sf::Keyboard::D:
            this->player.move(DIRECTION::right, this->level, elapsed);
            break;

        case sf::Keyboard::Space:
            this->player.speak(this->level.getId(), &this->pnjmgr);
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

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
