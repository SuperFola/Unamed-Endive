#include "../../debug.hpp"
#include <iostream>

#include "settings.hpp"

SettingsView::SettingsView() :
    View(SETTINGS_VIEW_ID)
{

}

#include "../constants.hpp"

#define __X event.mouseButton.x
#define __Y event.mouseButton.y


bool SettingsView::load()
{
    return true;
}

void SettingsView::render(sf::RenderWindow& window)
{

}

int SettingsView::process_event(sf::Event& event, sf::Time elapsed)
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

void SettingsView::update(sf::RenderWindow& window, sf::Time elapsed)
{
}
