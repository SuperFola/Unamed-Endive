#include<iostream>
#include "tilemap.hpp"
#include "../constants.hpp"

// public
int TileMap::load(const std::string& tileset_path)
{
    // load tileset !
    if (!this->tileset.loadFromFile(tileset_path))
        return 1;
    return 0;
}

int TileMap::load_map(sf::Vector2u tileSize, std::vector<Block*> tiles, unsigned int width, unsigned int height)
{
    // resize vertex array
    this->vertices.setPrimitiveType(sf::Quads);
    this->vertices.resize(width * height * 4);

    // fill vertex array, with one quad per tile
    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
        {
            int tileNumber = (tiles[i + j * width])->getId();

            int tu = tileNumber % (this->tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (this->tileset.getSize().x / tileSize.x);

            sf::Vertex* quad = &this->vertices[(i + j * width) * 4];

            quad[0].position = sf::Vector2f(i * TILE_SIZE, j * TILE_SIZE);
            quad[1].position = sf::Vector2f((i + 1) * TILE_SIZE, j * TILE_SIZE);
            quad[2].position = sf::Vector2f((i + 1) * TILE_SIZE, (j + 1) * TILE_SIZE);
            quad[3].position = sf::Vector2f(i * TILE_SIZE, (j + 1) * TILE_SIZE);

            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f(tu * tileSize.x + TILE_SIZE, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f(tu * tileSize.x + TILE_SIZE, tv * tileSize.y + TILE_SIZE);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y + TILE_SIZE);
        }

    return 0;
}
