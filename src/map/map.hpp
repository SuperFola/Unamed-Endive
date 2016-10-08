#ifndef DEF_MAP
#define DEF_MAP

#include <string>
#include <SFML/Graphics.hpp>
#include <vector>

#include "tilemap.hpp"
#include "blocks/block.hpp"
#include "../json/json.h"

class Map
{
private:
    // methods
    int load_map(const std::string&);
    // variables
    std::vector<std::vector<Block*>> level;
    std::vector<TileMap*> tmaps;
    Json::Value root;
    std::string tileset_path;
    std::string map_data_path;
    // default values for the moment
    int map_width;
    int map_height;

public:
    Map(std::string);
    int load();
    void render(sf::RenderWindow&);
    void render_top(sf::RenderWindow&);
    void update(sf::RenderWindow&, sf::Time);
    int getWidth();
    int getHeight();
    bool colliding_at(int, int);
};

#endif // DEF_MAP
