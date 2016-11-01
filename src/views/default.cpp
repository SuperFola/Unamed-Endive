#include <SFML/Graphics.hpp>
#include <iostream>

#include "default.hpp"
#include "../constants.hpp"

// public
DefaultView::DefaultView(const std::string& map_name) :
    View(DEFAULT_VIEW_ID)
    , level(map_name)
    , menu_hud()
{

}

bool DefaultView::load()
{
    this->level.load();
    if (!this->menu_hud.load())
        std::cout << "An error occured while loading the menu" <<std::endl;

    std::cout << "loading player" << std:: endl;
    Character player {"me", "assets/players/male/"};
    std::cout << "player loaded" << std::endl;

    return true;
}

void DefaultView::render(sf::RenderWindow& window)
{
    this->level.render(window);
    window.draw(this->player.getCurrentSprite());
    window.draw(this->player.rectangle);
    this->level.render_top(window);
    this->menu_hud.render(window);
}

void DefaultView::update(sf::RenderWindow& window, sf::Time elapsed)
{
    this->player.update(window, elapsed);
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
