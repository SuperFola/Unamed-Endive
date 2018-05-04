#include <iostream>

#include "map.hpp"
#include "../constants.hpp"
#ifdef PLATFORM_WIN
    #include <windows.h>
#endif // PLATFORM_WIN

// public
MapView::MapView() :
    View(MAP_VIEW_ID)
{

}

bool MapView::load()
{
    if (!mr.load())
        return false;

    return true;
}

void MapView::render(sf::RenderWindow& window)
{
    window.draw(this->mr);
}

int MapView::process_event(sf::Event& event, sf::Time elapsed)
{
    int new_view = -1;

    switch(event.type)
    {
    case sf::Event::KeyPressed:
        switch(event.key.code)
        {
        case sf::Keyboard::Escape:
            new_view = LAST_VIEW_ID;
            break;

        default:
            break;
        }
        break;

    case sf::Event::MouseButtonPressed:
        switch(event.mouseButton.button)
        {
        case sf::Mouse::Button::Left:
            this->mr.clic(int(m__X), int(m__Y));
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

    return new_view;
}

void MapView::update(sf::RenderWindow& window, sf::Time elapsed)
{
}
