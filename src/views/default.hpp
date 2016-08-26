#ifndef DEF_DEFAULT_VIEW
#define DEF_DEFAULT_VIEW

#include "view.hpp"

class DefaultView : public View
{
private:
    // variables
    sf::CircleShape shape;

public:
    // methods
    DefaultView();
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&) override;
};

#endif // DEF_DEFAULT_VIEW
