#ifndef DEF_MAP
#define DEF_MAP

#include <string>
#include <SFML/Graphics.hpp>
#include <vector>

#include "tilemap.hpp"
#include "blocks/block.hpp"
//#include "../json/jsoncpp.cpp"

class Map
{
private:
    // variables
    std::vector<Block> level;
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
    int getWidth();
    int getHeight();
    bool colliding_at(int, int);
};

#endif // DEF_MAP
