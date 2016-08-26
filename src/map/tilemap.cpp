#include "tilemap.hpp"

// public
int TileMap::load(const std::string& tileset_path, sf::Vector2u tileSize, const std::vector<int>& tiles, unsigned int width, unsigned int height)
{
    if (!this->tileset.loadFromFile(tileset_path))
        return 1;

    // resize vertex array
    this->vertices.setPrimitiveType(sf::Quads);
    this->vertices.resize(width * height * 4);

    // fill vertex array, with one quad per tile
    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
        {
            int tileNumber = tiles[i + j * width];

            int tu = tileNumber % (this->tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (this->tileset.getSize().x / tileSize.x);

            sf::Vertex* quad = &this->vertices[(i + j * width) * 4];

            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        }

    return 0;
}
