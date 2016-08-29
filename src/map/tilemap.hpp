#ifndef DEF_TILEMAP
#define DEF_TILEMAP

#include <SFML/Graphics.hpp>
#include <vector>

#include "blocks/block.hpp"

class TileMap : public sf::Drawable, public sf::Transformable
{
private:
    // methods
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        states.texture = &tileset;
        target.draw(this->vertices, states);
    }
    // variables
    sf::VertexArray vertices;
    sf::Texture tileset;

public:
    // methods
    int load(const std::string&, sf::Vector2u, std::vector<Block>&, unsigned int, unsigned int);
};

#endif // DEF_TILEMAP
