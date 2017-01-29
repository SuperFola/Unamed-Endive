#ifndef DEF_MAP
#define DEF_MAP

#include <string>
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>

#include "tilemap.hpp"
#include "blocks/block.hpp"
#include "../json/json.h"
#include "../constants.hpp"
#include "../abstract/functions.hpp"

class Map
{
private:
    // methods
    int load_map(const std::string&);
    // variables
    std::vector<std::vector<Block*>> level;
    std::vector<TileMap*> tmaps;
    std::map<int, int> spawns;
    std::map<int, int> tp;
    Json::Value root;
    std::string tileset_path;
    std::string map_data_path;
    int id;
    int map_width;
    int map_height;

public:
    Map(std::string);
    int load_map_at(const std::string& path="default");
    void render(sf::RenderWindow&);
    void micro_render(sf::RenderTexture&);
    void render_top(sf::RenderWindow&);
    void update(sf::RenderWindow&, sf::Time);
    int getWidth();
    int getHeight();
    int getId();
    bool colliding_at(int, int);
    bool is_spawn(int, int);
    bool is_tp(int, int);
    int getSpawnFrom(int);  // return a rpos, take a mid
    int getMapFromTp(int);  // return a mid, take a rpos
    bool smaller_than_window();
    void setBlockAttrib(int, std::string, bool);
};

#endif // DEF_MAP
