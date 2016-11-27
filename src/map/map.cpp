#include <iostream>
#include <fstream>

#include "map.hpp"

// public
Map::Map(std::string path) :
    tileset_path {"assets/tileset.png"}
    , pos(WIN_W / 2, WIN_H / 2)
    , map_width(16)
    , map_height(8)
{
    if (is_file_existing(path))
        std::cout << "Map file found : ";
    else
        std::cout << "Can not find map file : ";
    std::cout << path << std::endl;

    this->map_data_path = path;
    this->id = int(this->map_data_path.substr("assets/map/".size(), this->map_data_path.size() - ".umd".size()));
}

int Map::load()
{
    return this->load_map(this->map_data_path);
}

void Map::render(sf::RenderWindow& window)
{
    window.draw(*(this->tmaps[2]));
    window.draw(*(this->tmaps[1]));
}

void Map::render_top(sf::RenderWindow& window)
{
    // temp
    // window.draw(*(this->tmaps[0]));
}

void Map::render_chara(sf::Sprite& sprite_chara, Point& sprite_pos, sf::RenderWindow& window)
{
    sprite_chara.setPosition(this->pos.getX() + sprite_pos.getX(), this->pos.getY() + sprite_pos.getY());
    window.draw(sprite_chara);
}

void Map::update(sf::RenderWindow& window, sf::Time elapsed)
{
    for (int i=0; i < 3; i++)
    {
        this->tmaps[i]->setPosition(pos.getX(), pos.getY());
    }
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
    return 0;
}
