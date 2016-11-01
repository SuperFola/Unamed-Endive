#ifndef DEF_STATEMACHINE
#define DEF_STATEMACHINE
#define MAX_VIEWS 16
#define MAX_HISTORY 10

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

#include "../views/view.hpp"

class StateMachine {
private:
    // variables
    int current_view;
    std::vector<int> views {};
    std::vector<int> history {};

public:
    // methods
    StateMachine();
    void add_view(int);
    int getId();
    int change_view(int);
    int go_back_to_last_view();
};

#endif // DEF_STATEMACHINE
