#include <iostream>

#include "map.hpp"
#include "../constants.hpp"

// public
MapView::MapView() :
    View(MAP_VIEW_ID)
{

}

bool MapView::load()
{
    return true;
}

void MapView::render(sf::RenderWindow& window)
{

}

int MapView::process_event(sf::Event& event, sf::Time elapsed)
{
    return 1;
}

void MapView::update(sf::RenderWindow& window, sf::Time elapsed)
{

}
