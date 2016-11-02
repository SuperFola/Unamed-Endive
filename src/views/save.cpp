#include <iostream>

#include "save.hpp"
#include "../constants.hpp"

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
    return 1;
}

void SaveView::update(sf::RenderWindow& window, sf::Time elapsed)
{

}

