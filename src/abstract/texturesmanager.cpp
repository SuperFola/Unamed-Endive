#include <iostream>

#include "texturesmanager.hpp"

// public
TexturesManager::TexturesManager()
{

}

bool TexturesManager::loadTexture(std::string path)
{
    sf::Texture texture;
    if (!texture.loadFromFile(path))
    {
        std::cout << "Can not load texture at " << path << std::endl;
        return false;
    }
    this->textures.push_back(std::make_pair(path, std::move(texture)));
    return true;
}

sf::Texture TexturesManager::getTexture(std::string name)
{
    for (auto value : this->textures)
    {
        if (value.first == name)
        {
            return value.second;
        }
    }
}
