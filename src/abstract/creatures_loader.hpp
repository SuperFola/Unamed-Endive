#ifndef DEF_CREA_LOADER
#define DEF_CREA_LOADER

#include<string>
#include<vector>
#include <SFML/Graphics.hpp>

#include "container.hpp"
#include "../constants.hpp"

class CreaturesLoader
{
private:
    const std::string folder = "assets/creatures";
    Container<sf::Texture> textures;
    std::vector<std::string> files;
    int current = 0;

public:
    CreaturesLoader();
    bool load_next();
    sf::Texture& get(const std::string&);
};

#endif // DEF_CREA_LOADER
