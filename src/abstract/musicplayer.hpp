#ifndef DEF_MUSICPLAYER
#define DEF_MUSICPLAYER

#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include <map>

#include "soundmanager.hpp"

class MusicPlayer
{
private:
    bool loaded;
    SoundManager sdmgr;
    std::map<std::string, sf::Sound> sounds;
    std::string current;
    int _current;
    std::vector<std::string> sounds_name;

public:
    MusicPlayer();
    bool load();
    bool play(const std::string);
    void stop();
    std::string getNext();
    const std::string& getCurrentName();
};

#endif // DEF_MUSICPLAYER
