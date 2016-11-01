#ifndef DEF_TEXTURESMANAGER
#define DEF_TEXTURESMANAGER

#include <SFML/Graphics.hpp>
#include <string>
#include <tuple>
#include <map>

using dict_t = std::map<std::string, sf::Texture>;

class TexturesManager {
private:
    // variables
    dict_t textures;

public:
    // methods
    TexturesManager();
    bool loadTexture(const std::string&);
    void addTexture(const std::string&, sf::Texture&);
    sf::Texture& getTexture(const std::string&);
};

#endif // DEF_TEXTURESMANAGER
