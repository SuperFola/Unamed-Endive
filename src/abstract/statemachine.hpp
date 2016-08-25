#ifndef DEF_STATEMACHINE
#define DEF_STATEMACHINE
#define MAX_VIEWS 20

#include <vector>
#include <SFML/Graphics.hpp>

#include "../abstract/view.hpp"

class StateMachine {
private:
    // variables
    View current_view;
    std::vector<View> views {};

public:
    // methods
    StateMachine();
    int getId();
    void renderCurrentView(sf::Window&);
};

#endif // DEF_STATEMACHINE
