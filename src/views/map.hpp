#ifndef DEF_MAP_VIEW
#define DEF_MAP_VIEW

#include "view.hpp"

class MapView : public View
{
private:
    // variables

public:
    // methods
    MapView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
};

#endif // DEF_MAP_VIEW


