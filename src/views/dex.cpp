#include <iostream>

#include "dex.hpp"
#include "../constants.hpp"

// public
DexView::DexView() :
    View(DEX_VIEW_ID)
{

}

bool DexView::load()
{
    return true;
}

void DexView::render(sf::RenderWindow& window)
{

}

int DexView::process_event(sf::Event& event, sf::Time elapsed)
{
    return -1;
}

void DexView::update(sf::RenderWindow& window, sf::Time elapsed)
{

}
