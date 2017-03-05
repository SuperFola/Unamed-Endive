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

    this->default_tp.fromcase = -1;
    this->default_tp.tocase = -1;
    this->default_tp.tomap = -1;
}

int Map::load_map_at(std::string path)
{
    if (path != "default")
    {
        std::cout << this->id << " ";
        this->map_data_path = path;
        this->id = atoi(this->map_data_path.substr(11, this->map_data_path.size() - 4).data());
        std::cout << this->id << std::endl;
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
    window.draw(*this->tmaps[0]);
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
    bool ret_val = true;
    bool _tp = this->is_tp(tx, ty);

    if (rpos < this->map_height * this->map_width)
    {
        //if (!_tp)
            ret_val = this->level[COLLIDING_LAYER][rpos]->is_solid();
    }
    return ret_val;
}

int Map::post_colliding_test_to_check_tp(int tx, int ty)
{
    int nrpos = -1;
    int rpos = tx + ty * this->map_width;
    bool _tp = this->is_tp(tx, ty);

    if (_tp)
    {
        std::cout << "#changing from map " << this->id << "#" << std::endl;
        nrpos = this->findTpOnCase(rpos).tocase;
        std::cout << "0 ?= " << this->load_map_at("assets/map/" + to_string<int>(this->getMapFromTp(rpos)) + ".umd") << std::endl;
        std::cout << "# to map id " << this->getId() << std::endl << std::endl;
        std::cout << "#" << this->map_data_path << "#" << std::endl;
    }
    return nrpos;
}

TypeTp Map::findTpOnCase(int rpos)
{
    for (auto& o: this->tp)
    {
        if (o.fromcase == rpos)
            return o;
    }
    return this->default_tp;
}

// private
int Map::load_map(std::string& map_path)
{
    this->root.clear();
    this->tmaps.clear();
    this->level.clear();
    this->tp.clear();

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
    std::cout << "Tilemaps loaded" << std::endl;

    for (int i=0; i < this->root["tp"].size(); i++)
    {
        struct TypeTp ttp;
        ttp.tomap = this->root["tp"][i]["tomap"].asInt();
        ttp.tocase = this->root["tp"][i]["tocase"].asInt();
        ttp.fromcase = this->root["tp"][i]["oncase"].asInt();
        this->tp.push_back(ttp);
    }

    std::cout << "Tp loaded" << std::endl;

    std::cout << "mid " << this->id << std::endl;

    return 0;
}

bool Map::is_tp(int x, int y)
{
    int rpos = x + y * this->map_width;

    struct TypeTp ttp = this->findTpOnCase(rpos);

    if (ttp.fromcase != this->default_tp.fromcase && ttp.tocase != this->default_tp.tocase && ttp.tomap != this->default_tp.tomap)
        return true;
    return false;
}

int Map::getMapFromTp(int rpos)
{
    int mid;

    struct TypeTp ttp = this->findTpOnCase(rpos);
    mid = ttp.tomap;

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
