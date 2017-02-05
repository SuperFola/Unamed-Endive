#include <iostream>
#include <fstream>

#include "map.hpp"

// public
Map::Map(std::string path) :
    tileset_path {"assets/tileset.png"}
    , map_width(16)
    , map_height(8)
{
    if (is_file_existing(path))
        std::cout << "Map file found : ";
    else
        std::cout << "Can not find map file : ";
    std::cout << path << std::endl;

    this->map_data_path = path;
    this->id = atoi(this->map_data_path.substr(11, this->map_data_path.size() - 4).data());
}

int Map::load_map_at(const std::string& path)
{
    if (path != "default")
    {
        this->map_data_path = path;
        this->id = atoi(this->map_data_path.substr(11, this->map_data_path.size() - 4).data());
    }
    return this->load_map(this->map_data_path);
}

void Map::render(sf::RenderWindow& window)
{
    window.draw(*this->tmaps[2]);
    window.draw(*this->tmaps[1]);
}

void Map::micro_render(sf::RenderTexture& window)
{
    window.draw(*this->tmaps[2]);
    window.draw(*this->tmaps[1]);
}

void Map::render_top(sf::RenderWindow& window)
{
    // temp
    // window.draw(*this->tmaps[0]);
}

void Map::update(sf::RenderWindow& window, sf::Time elapsed)
{

}

int Map::getWidth()
{
    return this->map_width;
}

int Map::getHeight()
{
    return this->map_height;
}

int Map::getId()
{
    return this->id;
}

bool Map::colliding_at(int tx, int ty)
{
    int rpos = tx + ty * this->map_width;

    if (rpos < this->map_height * this->map_width)
        return this->level[COLLIDING_LAYER][rpos]->is_solid();
    return true;
}

// private
int Map::load_map(const std::string& map_path)
{
    std::ifstream config_doc(map_path);
    config_doc >> this->root;

    std::cout << "Loading map " << map_path << std::endl;
    this->map_width = this->root["width"].asInt();
    this->map_height = this->root["height"].asInt();

    const std::vector<std::string> maps = {"map", "map2", "map3"};

    for (const auto& map_name: maps)
    {
        std::vector<Block*> temp;
        for (int i=0; i < this->root[map_name].size(); ++i)
        {
            Block* block = new Block (
                this->root[map_name][i]["id"].asInt(),
                this->root[map_name][i]["colliding"].asBool()
            );
            temp.push_back(block);
        }
        this->level.push_back(temp);
    }
    std::cout << "Map loaded" << std::endl;

    for (int i=0; i < 3; i++)
    {
        TileMap* tmap = new TileMap();
        tmap->load(this->tileset_path);
        this->tmaps.push_back(tmap);

        if (this->tmaps[i]->load_map(sf::Vector2u(TILE_SIZE_IN_TILESET, TILE_SIZE_IN_TILESET), this->level[i], this->map_width, this->map_height))
            return 1;
    }

    for (int i=0; i < this->root["spawns"].size(); i++)
    {
        this->spawns[this->root["spawns"][i]["oncase"].asInt()] = this->root["spawns"][i]["frommap"].asInt();
    }

    for (int i=0; i < this->root["tp"].size(); i++)
    {
        this->tp[this->root["tp"][i]["oncase"].asInt()] = this->root["tp"][i]["tomap"].asInt();
    }

    return 0;
}

bool Map::is_spawn(int x, int y)
{
    int rpos = x + y * this->map_width;

    if (this->spawns.find(rpos) != this->spawns.end())
        return true;
    return false;
}

bool Map::is_tp(int x, int y)
{
    int rpos = x + y * this->map_width;

    if (this->tp.find(rpos) != this->tp.end())
        return true;
    return false;
}

int Map::getSpawnFromMap(int mid)
{
    int rpos = -1;

    for (auto& kv: this->spawns)
    {
        if (kv.second == mid)
        {
            rpos = kv.first;
            break;
        }
    }

    std::cout << "Spawn for map " << mid << " is on " << rpos << std::endl;

    return rpos;
}

int Map::getMapFromTp(int rpos)
{
    int mid;

    if (this->tp.find(rpos) == this->tp.end())
        mid = -1;
    else
        mid = this->tp.find(rpos)->second;

    std::cout << "Tp on " << rpos << " go to map " << mid << std::endl;

    return mid;
}

bool Map::smaller_than_window()
{
    return this->map_width <= WIN_W && this->map_height <= WIN_H;
}

void Map::setBlockAttrib(int rid, std::string identifier, bool value)
{
    if (identifier.compare("solid") != 0)
    {
        this->level[COLLIDING_LAYER][rid]->setSolid(value);
    }
}

int Map::getSpawnPosFromId(int sid)
{
    int rpos = -1;
    
    return rpos;
}


