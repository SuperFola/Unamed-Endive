#ifndef DEF_MAP_VIEW
#define DEF_MAP_VIEW

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

#include "view.hpp"
#include "../abstract/texturesmanager.hpp"
#include "../map/reader.hpp"

class MapView : public View
{
private:
    // variables
    MapReader mr;

public:
    // methods
    MapView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
};

#endif // DEF_MAP_VIEW


