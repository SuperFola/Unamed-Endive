#include <SFML/Graphics.hpp>
#include <iostream>

#include "default.hpp"
#include "../constants.hpp"

// public
DefaultView::DefaultView(const std::string& map_name) :
    View(DEFAULT_VIEW_ID)
    , level(map_name)
{

}

bool DefaultView::load()
{
    this->level.load();

    std::cout << std::endl << "loading player" << std:: endl;
    Character player {"me", "assets/players/male/"};
    std::cout << "player loaded" << std::endl;

    return true;
}

void DefaultView::render(sf::RenderWindow& window)
{
    this->level.render(window);
    window.draw(this->player.getCurrentSprite());
}

void DefaultView::update(sf::RenderWindow& window, sf::Time elapsed)
{
    this->player.update(window, elapsed);
}

int DefaultView::process_event(sf::Event& event, sf::Time elapsed)
{
    switch(event.type)
    {
    case sf::Event::KeyPressed:
        std::cout << "moving" << std::endl;
        switch(event.key.code)
        {
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

    // we don't need to change the view
    return -1;
}
