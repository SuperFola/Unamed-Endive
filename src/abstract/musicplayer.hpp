#ifndef DEF_MUSICPLAYER
#define DEF_MUSICPLAYER

#include <SFML/Audio.hpp>
#include <string>
#include <vector>

class MusicPlayer
{
private:
    bool loaded;
    bool state;
    sf::Music music;
    std::string current;
    int _current;
    int volume;
    std::vector<std::string> sounds_name;

public:
    MusicPlayer();
    bool load();
    bool getState();
    bool load(const std::string&);
    bool play(const std::string&);
    void stop();
    std::string getNext();
    const std::string& getCurrentName();
    void setVolume(int);
};

#endif // DEF_MUSICPLAYER
