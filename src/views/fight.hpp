#ifndef DEF_FIGHT_VIEW
#define DEF_FIGHT_VIEW

#include <SFML/Graphics.hpp>

#include "view.hpp"

class FightView : public View
{
private:

public:
    FightView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
};

#endif // DEF_FIGHT_VIEW
