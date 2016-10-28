#include <SFML/Graphics.hpp>
#include <iostream>

#include "menu.hpp"
#include "../constants.hpp"

// public
MenuView::MenuView()
{
}

bool MenuView::load()
{
    return true;
}

void MenuView::render(sf::RenderWindow& window)
{
}

void MenuView::update(sf::RenderWindow& window, sf::Time elapsed)
{
}

int MenuView::process_event(sf::Event& event, sf::Time elapsed)
{
    switch(event.type)
    {
    case sf::Event::KeyPressed:
        switch(event.key.code)
        {
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
