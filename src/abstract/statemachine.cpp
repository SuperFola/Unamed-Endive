#include <iostream>

#include "statemachine.hpp"
#include "functions.hpp"

// public
StateMachine::StateMachine() : current_view(-1)
{
    this->views.reserve(MAX_VIEWS);
    this->history.reserve(MAX_HISTORY);
}

void StateMachine::add_view(int view)
{
    if (this->views.size() == 0)
    {
        this->current_view = view;
        std::cout << "The view with id " << view << " is now the default one" << std::endl;
    }
    this->views.push_back(view);

    std::cout << "Adding view id " << view << " to the SM" << std::endl << "    ";
    for (auto value : this->views) {std::cout << value << " - ";}
    std::cout << std::endl;
}

int StateMachine::getId()
{
    return this->current_view;
}

int StateMachine::change_view(int new_view)
{
    if (new_view == -1)
        return 0;

    for (auto value : this->views)
    {
        if (value == new_view)
        {
            this->history.push_back(value);
            this->current_view = new_view;
            std::cout << "Changing view from id " << value << " to id " << new_view << std::endl;
            return 0;
        }
    }

    return 1;  // unable to find the correct view
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
            return 1;  // can not pop
        }
    }
    return 1; // history is empty
}
