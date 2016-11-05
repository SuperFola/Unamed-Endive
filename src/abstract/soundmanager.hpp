#ifndef DEF_SOUNDMANAGER
#define DEF_SOUNDMANAGER

#include <SFML/Audio.hpp>
#include <string>
#include <map>

using dict_b = std::map<std::string, sf::SoundBuffer>;

class SoundManager
{
private:
    dict_b sounds;

public:
    SoundManager();
    bool addSound(const std::string&, sf::SoundBuffer);
    sf::SoundBuffer& getBuffer(const std::string&);
};

#endif // DEF_SOUNDMANAGER
