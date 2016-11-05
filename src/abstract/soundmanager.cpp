#include <iostream>

#include "soundmanager.hpp"

SoundManager::SoundManager()
{

}

bool SoundManager::addSound(const std::string& name, sf::SoundBuffer buffer)
{
    this->sounds.emplace(name, buffer);
    return true;
}

sf::SoundBuffer& SoundManager::getBuffer(const std::string& name)
{
    auto it = this->sounds.find(name);

    if (it == this->sounds.end())
        std::cout << "Can not find sound buffer with name " << name << std::endl;
    else
        return it->second;
}
