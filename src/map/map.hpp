#ifndef DEF_MAP
#define DEF_MAP

#include <string>
#include <SFML/Graphics.hpp>
#include <vector>

#include "tilemap.hpp"

class Map
{
private:
    // variables
    const std::vector<int> level =
    {
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
        0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
        0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
        0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
        2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
        0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    }; // we will load it from a file in the future
    TileMap tmap;
    std::string tileset_path;
    // default values for the moment
    int map_width;
    int map_height;

public:
    Map();
    int load();
    void render(sf::RenderWindow&);
    void update(sf::RenderWindow&, sf::Time);
};

#endif // DEF_MAP
