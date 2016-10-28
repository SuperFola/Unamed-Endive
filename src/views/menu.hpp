#ifndef DEF_DEFAULT_VIEW
#define DEF_DEFAULT_VIEW

#include <string>
#include <vector>

#include "view.hpp"

class MenuView : public View
{
private:
    // variables
    std::vector<std::string> menus;

public:
    // methods
    MenuView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
};

#endif // DEF_DEFAULT_VIEW
