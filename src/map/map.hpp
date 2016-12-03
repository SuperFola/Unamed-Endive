#ifndef DEF_MAP
#define DEF_MAP

#include <string>
#include <SFML/Graphics.hpp>
#include <vector>

#include "tilemap.hpp"
#include "blocks/block.hpp"
#include "../json/json.h"
#include "../constants.hpp"
#include "../abstract/functions.hpp"
#include "../abstract/point.hpp"

class Map
{
private:
    // methods
    int load();
    int load_map(const std::string&);
    // variables
    std::vector<std::vector<Block*>> level;
    std::vector<TileMap*> tmaps;
    Json::Value root;
    std::string tileset_path;
    std::string map_data_path;
    Point pos;
    int id;
    int map_width;
    int map_height;

public:
    Map(std::string);
    int load_map_at(const std::string& path="default");
    void render(sf::RenderWindow&);
    void render_top(sf::RenderWindow&);
    void render_chara(sf::Sprite&, Point&, sf::RenderWindow&);
    void update(sf::RenderWindow&, sf::Time);
    int getWidth();
    int getHeight();
    int getId();
    bool colliding_at(int, int);
};

#endif // DEF_MAP
