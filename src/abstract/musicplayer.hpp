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
    SoundManager sdmgr;
    std::map<std::string, sf::Sound> sounds;
    std::string current;
    int _current;
    std::vector<std::string> sounds_name;
    bool loaded;

public:
    MusicPlayer();
    bool load();
    void play(const std::string);
    void stop();
    std::string getNext();
};

#endif // DEF_MUSICPLAYER
