#ifndef DEF_STATEMACHINE
#define DEF_STATEMACHINE

#define MAX_VIEWS 16
#define MAX_HISTORY 10

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../views/view.hpp"
#include "../views/default.hpp"
#include "../views/creatures.hpp"
#include "../views/dex.hpp"
#include "../views/save.hpp"
#include "../views/inventory.hpp"
#include "../views/map.hpp"

class StateMachine {
private:
    // variables
    int loaded;
    int current_view;
    std::vector<std::unique_ptr<View>> views;
    std::vector<int> history;

public:
    // methods
    StateMachine();
    bool load();
    int getId();
    int change_view(int);
    int go_back_to_last_view();
    int process_event_current(sf::Event&, sf::Time);
    void render_current(sf::RenderWindow&);
    void update_current(sf::RenderWindow&, sf::Time);
};

#endif // DEF_STATEMACHINE
