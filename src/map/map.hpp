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
#include "type_tp.hpp"

class Map
{
private:
    // methods
    int load_map(std::string&);
    // variables
    std::vector<std::vector<Block*>> level;
    std::vector<TileMap*> tmaps;
    std::vector<struct TypeTp> tp;
    Json::Value root;
    std::string tileset_path;
    std::string map_data_path;
    int id;
    int map_width;
    int map_height;
    struct TypeTp default_tp;

public:
    Map(std::string);
    ~Map();
    void setMapPath(const std::string&);
    int load_map_at(std::string path="default");
    void render(sf::RenderTexture&);
    void micro_render(sf::RenderTexture&);
    void render_top(sf::RenderTexture&);
    void update(sf::RenderWindow&, sf::Time);
    int getWidth();
    int getHeight();
    int getId();
    bool colliding_at(int, int);
    int post_colliding_test_to_check_tp(int, int);
    bool is_tp(int, int);
    TypeTp findTpOnCase(int); // take a rpos
    int getMapFromTp(int);  // return a mid, take a rpos
    bool smaller_than_window();
    void setBlockAttrib(int, std::string, bool);
};

#endif // DEF_MAP
