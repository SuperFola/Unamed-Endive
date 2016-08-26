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
        this->current_view = view;
    this->views.push_back(view);
}

int StateMachine::getId()
{
    return this->current_view;
}

int StateMachine::change_view(int new_view)
{
    for (auto value : this->views)
    {
        if (value == new_view)
        {
            this->history.push_back(value);
            this->current_view = new_view;
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
            return 0;
        }
        else
        {
            return 1;  // can not pop
        }
    }
    return 1; // history is empty
}
