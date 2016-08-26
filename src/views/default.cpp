#include <SFML/Graphics.hpp>

#include "default.hpp"
#include "../constants.hpp"

// public
DefaultView::DefaultView() : View(DEFAULT_VIEW_ID)
{
    this->shape = sf::CircleShape(100.f);
    this->shape.setFillColor(sf::Color::Green);
}

void DefaultView::render(sf::RenderWindow& window)
{
    window.draw(this->shape);
}

int DefaultView::process_event(sf::Event& event)
{
    switch(event.type)
    {
    case sf::Event::KeyPressed:
        switch(event.key.code)
        {
        case sf::Keyboard::Space:
            this->shape.setFillColor(sf::Color::Blue);
            break;

        case sf::Keyboard::A:
            this->shape.setFillColor(sf::Color::Green);
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

    return -1;
}
