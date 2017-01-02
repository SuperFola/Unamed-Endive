#ifndef MAP_READER
#define MAP_READER

#include<string>
#include<vector>
#include<SFML/Graphics.hpp>

#include "../json/json.h"
#include "../constants.hpp"

class MapReader : public sf::Drawable, public sf::Transformable
{
private:
    // methods
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(this->bg, states);
        states.transform *= getTransform();
        states.texture = &tileset;
        target.draw(this->vertices, states);

        if (this->selected != -1)
            target.draw(this->text, states);
    }
    // variables
    sf::VertexArray vertices;
    sf::Texture tileset;
    Json::Value root;
    Json::Value content;
    std::vector<int> tiles;
    sf::Texture t_bg;
    sf::Sprite bg;
    sf::Font font;
    sf::Text text;
    int selected;

public:
    MapReader();
    bool load();
    void clic(int, int);
};

#endif // MAP_READER

