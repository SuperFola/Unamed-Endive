#include "map.hpp"
#include "../constants.hpp"

// public
Map::Map() : tileset_path {"assets/tileset.png"}, map_width(16), map_height(8)
{
}

int Map::load()
{
    if (this->tmap.load(this->tileset_path, sf::Vector2u(TILE_SIZE, TILE_SIZE), this->level, this->map_width, this->map_height))
        return 1;
    return 0;
}

void Map::render(sf::RenderWindow& window)
{
    window.draw(this->tmap);
}

void Map::update(sf::RenderWindow& window, sf::Time elapsed)
{

}
