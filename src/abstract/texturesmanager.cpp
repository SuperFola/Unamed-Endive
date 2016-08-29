#include <iostream>

#include "texturesmanager.hpp"

// public
TexturesManager::TexturesManager()
{

}

bool TexturesManager::loadTexture(const std::string& path)
{
    sf::Texture texture;
    if (!texture.loadFromFile(path))
    {
        std::cout << "Can not load texture at " << path << std::endl;
        return false;
    }
    this->textures.emplace(path, texture);
    return true;
}

sf::Texture& TexturesManager::getTexture(const std::string& name)
{
    auto it = this->textures.find(name);

    if (it == this->textures.end())
        std::cout << "Can not find texture with name " << name << std::endl;
    else
        return it->second;
}

void TexturesManager::addTexture(const std::string& name, sf::Texture& texture)
{
    this->textures.emplace(name, texture);
}
