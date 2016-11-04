#ifndef DEF_TEXTURESMANAGER
#define DEF_TEXTURESMANAGER

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

using dict_t = std::map<std::string, sf::Texture>;

class TexturesManager {
private:
    // variables
    dict_t textures;

public:
    // methods
    TexturesManager();
    void addTexture(const std::string&, sf::Texture&);
    sf::Texture& getTexture(const std::string&);
};

#endif // DEF_TEXTURESMANAGER
