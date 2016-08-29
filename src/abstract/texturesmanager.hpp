#ifndef DEF_TEXTURESMANAGER
#define DEF_TEXTURESMANAGER

#include <SFML/Graphics.hpp>
#include <string>
#include <tuple>

using dict = std::vector<std::pair<std::string, sf::Texture>>;

class TexturesManager {
private:
    // variables
    dict textures;

public:
    // methods
    TexturesManager();
    bool loadTexture(std::string);
    sf::Texture getTexture(std::string);
};

#endif // DEF_TEXTURESMANAGER
