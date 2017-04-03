#include <iostream>

#include "shop.hpp"
#include "../constants.hpp"

#define __X event.mouseButton.x
#define __Y event.mouseButton.y

// public
ShopView::ShopView() :
    View(SHOP_VIEW_ID)
{

}

bool ShopView::load()
{
    return true;
}

void ShopView::render(sf::RenderWindow& window)
{

}

int ShopView::process_event(sf::Event& event, sf::Time elapsed)
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

void ShopView::update(sf::RenderWindow& window, sf::Time elapsed)
{
}
