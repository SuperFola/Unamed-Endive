#ifndef DEF_INVENT_VIEW
#define DEF_INVENT_VIEW

#include "view.hpp"

class InventView : public View
{
private:
    // variables

public:
    // methods
    InventView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
};

#endif // DEF_INVENT_VIEW




