#include <SFML/Graphics.hpp>
#include <iostream>

#include "default.hpp"

// public
DefaultView::DefaultView() :
    View(DEFAULT_VIEW_ID)
    , pnj("vader", "Je suis Vader, un commander d'El Padrino !", PNJkind::special)
    , level("assets/map/1-1441.umd")
{
}

bool DefaultView::load()
{
    this->level.load();
    if (!this->menu_hud.load())
    {
        std::cout << "An error occured while loading the menu" <<std::endl;
        return false;
    }

    this->pnj.setDisplayName("Vader");
    if (!this->pnj.load())
    {
        std::cout << "An error occured while loading a test pnj" <<std::endl;
        return false;
    }

    this->player.setName("me");
    if (!this->player.load())
    {
        std::cout << "An error occured while loading the player" <<std::endl;
        return false;
    }

    return true;
}

void DefaultView::render(sf::RenderWindow& window)
{
    this->level.render(window);
    window.draw(this->player.getCurrentSprite());
    window.draw(this->player.rectangle);
    this->pnj.render(window);  // testing
    this->level.render_top(window);
    this->menu_hud.render(window);
}

void DefaultView::update(sf::RenderWindow& window, sf::Time elapsed)
{
    this->player.update(window, elapsed);
    this->pnj.update(window, elapsed); // testing
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
            this->player.move(DIR::up, this->level, elapsed);
            break;

        case sf::Keyboard::S:
            this->player.move(DIR::down, this->level, elapsed);
            break;

        case sf::Keyboard::Q:
            this->player.move(DIR::left, this->level, elapsed);
            break;

        case sf::Keyboard::D:
            this->player.move(DIR::right, this->level, elapsed);
            break;

        case sf::Keyboard::Space:
            this->pnj.speak();
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

Character* DefaultView::getCharacter()
{
    return &(this->player);
}
