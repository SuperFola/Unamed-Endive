#include <iostream>

#include "inventory.hpp"
#include "../constants.hpp"

// public
InventView::InventView() :
    View(INVENTORY_VIEW_ID)
{

}

bool InventView::load()
{
    return true;
}

void InventView::render(sf::RenderWindow& window)
{

}

int InventView::process_event(sf::Event& event, sf::Time elapsed)
{
    return 1;
}

void InventView::update(sf::RenderWindow& window, sf::Time elapsed)
{

}
