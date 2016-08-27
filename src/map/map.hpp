#ifndef DEF_MAP
#define DEF_MAP

#include <string>
#include <SFML/Graphics.hpp>
#include <vector>

#include "tilemap.hpp"
//#include "../json/jsoncpp.cpp"

class Map
{
private:
    // variables
    std::vector<int> level =
    {
        0,  1,  2, 68, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        3,  4,  5, 68, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
        6,  7,  8, 68, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
        68, 68, 68, 70, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
        0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
        0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
        2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
        0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    }; // we will load it from a file in the future
    TileMap tmap;
    //Json::Value root;
    std::string tileset_path;
    std::string map_data_path;
    // default values for the moment
    int map_width;
    int map_height;

public:
    Map(std::string);
    int load();
    void render(sf::RenderWindow&);
    void update(sf::RenderWindow&, sf::Time);
};

#endif // DEF_MAP
