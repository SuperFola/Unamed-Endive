#include <iostream>

#include "statemachine.hpp"
#include "functions.hpp"

// public
StateMachine::StateMachine() :
    loaded(0)
    , current_view(-1)
{
    this->views.reserve(MAX_VIEWS);
    this->history.reserve(MAX_HISTORY);
}

bool StateMachine::load()
{
    switch(this->loaded)
    {
    case 0:
        this->views.emplace_back(std::make_unique<DefaultView>());
        this->views[this->views.size() - 1]->load();
        break;

    case 1:
        this->views.emplace_back(std::make_unique<CreaView>());
        this->views[this->views.size() - 1]->load();
        break;

    case 2:
        this->views.emplace_back(std::make_unique<DexView>());
        this->views[this->views.size() - 1]->load();
        break;

    case 3:
        this->views.emplace_back(std::make_unique<SaveView>());
        this->views[this->views.size() - 1]->load();
        break;

    case 4:
        this->views.emplace_back(std::make_unique<InventView>());
        this->views[this->views.size() - 1]->load();
        break;

    case 5:
        this->views.emplace_back(std::make_unique<MapView>());
        this->views[this->views.size() - 1]->load();
        break;
    }
    std::cout << "Loading view " << this->views[this->views.size() - 1]->getId() << std::endl;
    this->loaded++;

    return this->loaded == 6;
}

int StateMachine::getId()
{
    return this->current_view;
}

int StateMachine::change_view(int new_view)
{
    if (new_view == -1)
        return 0;

    for (auto&& value : this->views)
    {
        if (value->getId() == new_view)
        {
            this->history.push_back(this->current_view);
            this->current_view = new_view;
            std::cout << "Changing view from id " << this->history[this->history.size() - 1] << " to id " << new_view << std::endl;
            return 1;
        }
    }

    return -1;  // unable to find the correct view
}

int StateMachine::go_back_to_last_view()
{
    if (this->history.size() != 0)
    {
        int latest = pop<int>(&this->history, -1);
        if (latest != -1)
        {
            this->current_view = latest;
            std::cout << "Going back to view id " << latest << std::endl;
            return 0;
        }
        else
        {
            return -1;  // can not pop
        }
    }
    return -1; // history is empty
}

int StateMachine::process_event_current(sf::Event& event, sf::Time elapsed)
{
    for (auto&& element: this->views) {
        if (element->getId() == this->current_view)
            return element->process_event(event, elapsed);
    }
    return -1;
}

void StateMachine::render_current(sf::RenderWindow& window)
{
    for (auto&& element: this->views) {
        if (element->getId() == this->current_view)
            element->render(window);
    }
}

void StateMachine::update_current(sf::RenderWindow& window, sf::Time elapsed)
{
    for (auto&& element: this->views) {
        if (element->getId() == this->current_view)
            element->update(window, elapsed);
    }
}
