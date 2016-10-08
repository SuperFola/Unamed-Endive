#ifndef DEF_DEFAULT_VIEW
#define DEF_DEFAULT_VIEW

#include <string>

#include "view.hpp"
#include "../map/map.hpp"
#include "../entities/character.hpp"

class DefaultView : public View
{
private:
    // variables
    Character player;
    Map level;

public:
    // methods
    DefaultView(const std::string& ="assets/map/1-1441.umd");
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
};

#endif // DEF_DEFAULT_VIEW
