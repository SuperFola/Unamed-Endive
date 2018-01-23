#include <iostream>
#include <fstream>
#include "../../debug.hpp"

#include "map.hpp"
#include "../scripting/scripting.hpp"

// public
Map::Map(std::string path) :
    tileset_path {"assets/tileset.png"}
    , map_width(16)
    , map_height(8)
{
    if (is_file_existing(path))
        DebugLog(SH_OK, "Map file found : " << path);
    else
        DebugLog(SH_ERR, "Can not find map file at : " << path);

    this->map_data_path = path;
    this->id = atoi(this->map_data_path.substr(11, this->map_data_path.size() - 4).data());

    this->default_tp.fromcase = -1;
    this->default_tp.tocasex = -1;
    this->default_tp.tocasey = -1;
    this->default_tp.tomap = -1;
}

void Map::setMapPath(const std::string& npath)
{
    this->map_data_path = npath;
    this->id = atoi(this->map_data_path.substr(11, this->map_data_path.size() - 4).data());
}

int Map::load_map_at(std::string path)
{
    if (path != "default")
    {
        DebugLog(SH_SPE, "Changing map. Current id : " << this->id);
        this->map_data_path = path;
        this->id = atoi(this->map_data_path.substr(11, this->map_data_path.size() - 4).data());
        DebugLog(SH_SPE, "Changed map. New id : " << this->id);
    }
    return this->load_map(this->map_data_path);
}

void Map::render(sf::RenderTexture& window)
{
    window.draw(*this->tmaps[2]);
    window.draw(*this->tmaps[1]);
}

void Map::micro_render(sf::RenderTexture& window)
{
    window.draw(*this->tmaps[2]);
    window.draw(*this->tmaps[1]);
}

void Map::render_top(sf::RenderTexture& window)
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
    // if we are in range for the tile to get
    if (rpos < this->map_height * this->map_width && tx < (this->map_width - 1) && ty < (this->map_height - 1))
        { ret_val = this->level[COLLIDING_LAYER][rpos]->is_solid(); }
    return ret_val;
}

int Map::post_colliding_test_to_check_tp(int tx, int ty)
{
    int nrpos = -1;
    int rpos = tx + ty * this->map_width;
    bool _tp = this->is_tp(tx, ty);

    if (_tp)
    {
        TypeTp ttp = this->findTpOnCase(rpos);
        DebugLog(SH_SPE, "0 ?= " << this->load_map_at("assets/map/" + to_string<int>(this->getMapFromTp(rpos)) + ".umd"));
        nrpos = ttp.tocasex + ttp.tocasey * this->getWidth();
        if (nrpos != -1)
        {
            PyScripting::run_code(("trigger_event(" +
                                   to_string<int>(this->getId()) + "," +
                                   to_string<int>(tx / TILE_SIZE) + "," +
                                   to_string<int>(ty / TILE_SIZE) + "," +
                                   "'autorun')").c_str());
        }
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
    CLEAR_PTR_VECT(this->tmaps)
    for (auto& v: this->level)
    {
        CLEAR_PTR_VECT(v)
    }
    this->level.clear();
    this->tp.clear();

    std::ifstream config_doc(map_path);
    config_doc >> this->root;

    DebugLog(SH_INFO, "Loading map");
    this->map_width = this->root["width"].asInt();
    this->map_height = this->root["height"].asInt();
    DebugLog(SH_INFO, "Width : " << this->map_width << "  Height : " << this->map_height);

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
        DebugLog(SH_INFO, "Loaded layer '" << map_name << "' as layer number " << this->level.size() - 1);
    }
    DebugLog(SH_OK, "Map loaded");

    for (int i=0; i < 3; i++)
    {
        TileMap* tmap = new TileMap();
        tmap->load(this->tileset_path);
        this->tmaps.push_back(tmap);

        if (this->tmaps[i]->load_map(sf::Vector2u(TILE_SIZE_IN_TILESET, TILE_SIZE_IN_TILESET), this->level[i], this->map_width, this->map_height))
            return 1;
    }
    DebugLog(SH_OK, "Tilemaps loaded");

    for (int i=0; i < this->root["tp"].size(); i++)
    {
        struct TypeTp ttp;
        ttp.tomap = this->root["tp"][i]["tomap"].asInt();
        ttp.tocasex = this->root["tp"][i]["tocasex"].asInt();
        ttp.tocasey = this->root["tp"][i]["tocasey"].asInt();
        ttp.fromcase = this->root["tp"][i]["oncase"].asInt();
        this->tp.push_back(ttp);
    }

    config_doc.close();

    DebugLog(SH_OK, "Tp loaded");

    return 0;
}

bool Map::is_tp(int x, int y)
{
    int rpos = x + y * this->map_width;
    struct TypeTp ttp = this->findTpOnCase(rpos);

    if (ttp.fromcase != this->default_tp.fromcase && ttp.tocasex != this->default_tp.tocasex &&
            ttp.tocasey != this->default_tp.tocasey && ttp.tomap != this->default_tp.tomap)
        return true;
    return false;
}

int Map::getMapFromTp(int rpos)
{
    int mid;
    struct TypeTp ttp = this->findTpOnCase(rpos);
    mid = ttp.tomap;
    DebugLog(SH_INFO, "Tp on " << rpos << " go to map " << mid);
    return mid;
}

bool Map::smaller_than_window()
{
    return this->map_width * TILE_SIZE <= WIN_W && this->map_height * TILE_SIZE <= WIN_H;
}

void Map::setBlockAttrib(int rid, std::string identifier, int value)
{
    if (identifier.compare("solid") != 0)
    {
        this->level[COLLIDING_LAYER][rid]->setSolid(bool(value));
    }
    if (identifier.compare("id") != 0)
    {
        this->level[COLLIDING_LAYER][rid]->setId(value);
        // reload the layer
        this->tmaps[COLLIDING_LAYER]->load_map(sf::Vector2u(TILE_SIZE_IN_TILESET, TILE_SIZE_IN_TILESET)
                                               , this->level[COLLIDING_LAYER], this->map_width, this->map_height);
    }
}

int Map::getTileAt(int rpos)
{
    if (0 <= rpos < this->map_height * this->map_width)
        return this->level[COLLIDING_LAYER][rpos]->getId();
    return -1;
}

Map::~Map()
{
    CLEAR_PTR_VECT(this->tmaps)
    for (auto& v: this->level)
    {
        CLEAR_PTR_VECT(v)
    }
    this->level.clear();
}
