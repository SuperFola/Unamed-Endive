#include "statemachine.hpp"

// private

// public
StateMachine::StateMachine()
{
    this->views.reserve(MAX_VIEWS);
}

int StateMachine::getId()
{
    return this->current_view.getId();
}

void StateMachine::renderCurrentView(sf::Window& window)
{

}
