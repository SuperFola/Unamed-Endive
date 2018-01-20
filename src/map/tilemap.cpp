#include<iostream>

#include "tilemap.hpp"
#include "../constants.hpp"
#include "../../debug.hpp"

bool TileMap::loaded_tileset = false;
sf::Texture TileMap::tileset;

// public
int TileMap::load(const std::string& tileset_path)
{
    // load tileset !
    if (!TileMap::loaded_tileset)
    {
        sf::Image image;
        if (!image.loadFromFile(tileset_path))
            DebugLog(SH_ERR, "Unable to open " << tileset_path);
        else
        {
            image.createMaskFromColor(sf::Color(  0, 183, 239, 255));
            image.createMaskFromColor(sf::Color(255, 242,   0, 255));
            if (!TileMap::tileset.loadFromImage(image))
                return 1;
        }
        TileMap::loaded_tileset = true;
    }
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

            if (tileNumber == -1 || tileNumber == 99999)
                tileNumber = TRANSPARENT_TILE;

            int tu = tileNumber % (TileMap::tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (TileMap::tileset.getSize().x / tileSize.x);

            sf::Vertex* quad = &this->vertices[(i + j * width) * 4];

            // on définit ses quatre coins
            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            // on définit ses quatre coordonnées de texture
            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        }

    return 0;
}
