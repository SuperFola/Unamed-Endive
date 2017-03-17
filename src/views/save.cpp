#include <iostream>

#include "save.hpp"
#include "../constants.hpp"

#define __X event.mouseButton.x
#define __Y event.mouseButton.y

// public
SaveView::SaveView() :
    View(SAVING_VIEW_ID)
{

}

bool SaveView::load()
{
    return true;
}

void SaveView::render(sf::RenderWindow& window)
{

}

int SaveView::process_event(sf::Event& event, sf::Time elapsed)
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

void SaveView::update(sf::RenderWindow& window, sf::Time elapsed)
{

}

void SaveView::setdefv(DefaultView* _defv)
{
    this->defv = _defv;
}

void SaveView::save()
{
    this->defv->getCharacter()->save();

    Json::Value root;
    root["id"] = this->defv->getMap()->getId();

    std::ofstream output("saves/" + this->defv->getCharacter()->getName() + "/map.json");
    output << root;
}
