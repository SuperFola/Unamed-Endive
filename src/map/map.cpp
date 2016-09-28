#include <iostream>
#include <fstream>

#include "map.hpp"
#include "../constants.hpp"

bool is_file_existing(const std::string& file)
{
    std::ifstream reader(file.c_str());
    return !reader.fail();
}

// public
Map::Map(std::string path) : tileset_path {"assets/tileset.png"}, map_width(16), map_height(8)
{
    if (is_file_existing(path))
        std::cout << "Map file found : ";
    else
        std::cout << "Can not find map file : ";
    std::cout << path << std::endl;

    for (int i=0; i < this->map_width * this->map_height; i++)
        this->level.push_back(Block(68, false));
    this->map_data_path = path;

    std::ifstream config_doc(this->map_data_path);
    config_doc >> this->root;

    this->map_width = this->root["width"].asInt();
    this->map_height = this->root["height"].asInt();
    std::vector<Block> loaded_map;
    for (int i=0; i < this->root["level"].size(); ++i)
        loaded_map.push_back(Block(this->root["level"][i]["id"].asInt(), this->root["level"][i]["collide"] == 1));
    this->level = loaded_map;
}

int Map::load()
{
    if (this->tmap.load(this->tileset_path, sf::Vector2u(TILE_SIZE_IN_TILESET, TILE_SIZE_IN_TILESET), this->level, this->map_width, this->map_height))
        return 1;
    return 0;
}

void Map::render(sf::RenderWindow& window)
{
    window.draw(this->tmap);
}

void Map::update(sf::RenderWindow& window, sf::Time elapsed)
{
    this->tmap.setPosition((WIN_W) / 2, (WIN_H) / 2);
}

int Map::getWidth()
{
    return this->map_width;
}

int Map::getHeight()
{
    return this->map_height;
}

bool Map::colliding_at(int tx, int ty)
{
    int rpos = tx + ty * this->map_width;

    if (rpos < this->map_height * this->map_width)
        return this->level[rpos].is_solid();
    return true;
}
