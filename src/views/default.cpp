#include <SFML/Graphics.hpp>
#include <iostream>

#include "default.hpp"

// public
DefaultView::DefaultView() :
    View(DEFAULT_VIEW_ID)
    , level("assets/map/1.umd")
{
}

bool DefaultView::load()
{
    this->level.load_map_at();  // empty will cause to load the map given by default
    if (!this->menu_hud.load())
    {
        std::cout << "An error occured while loading the menu" << std::endl;
        return false;
    }

    this->pnjmgr.add_pnj_on_map(this->level.getId(), "vader", "Je suis Vader, un commander d'El Padrino !", PNJkind::special, "Vader");

    this->player.setName("me");
    if (!this->player.load())
    {
        std::cout << "An error occured while loading the player" << std::endl;
        return false;
    }

    return true;
}

void DefaultView::render(sf::RenderWindow& window)
{
    this->level.render(window);
    this->level.render_chara(this->player.getCurrentSprite(), this->player.getPos(), window);
    for (int i=0; i < this->pnjmgr.countPNJonMap(this->level.getId()); i++)
    {
        this->level.render_chara(
                                 this->pnjmgr.getPNJonMap(this->level.getId(), i).getCurrentSprite()
                                 , this->pnjmgr.getPNJonMap(this->level.getId(), i).getPos()
                                 , window);
        this->pnjmgr.getPNJonMap(this->level.getId(), i).render(window);
    }
    this->level.render_top(window);
    this->menu_hud.render(window);
}

void DefaultView::update(sf::RenderWindow& window, sf::Time elapsed)
{
    this->player.update(window, elapsed);
    this->level.update(window, elapsed);
    this->pnjmgr.update(this->level.getId(), window, elapsed);
    this->menu_hud.update(window, elapsed);
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
        case sf::Keyboard::RShift:
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
