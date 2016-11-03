#include <iostream>

#include "creatures.hpp"
#include "../constants.hpp"

// public
CreaView::CreaView() :
    View(MYCREATURES_VIEW_ID)
{

}

bool CreaView::load()
{
    return true;
}

void CreaView::render(sf::RenderWindow& window)
{

}

int CreaView::process_event(sf::Event& event, sf::Time elapsed)
{
    return -1;
}

void CreaView::update(sf::RenderWindow& window, sf::Time elapsed)
{

}
