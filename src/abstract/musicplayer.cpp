#include <iostream>
#include "../../debug.hpp"

#include "musicplayer.hpp"

MusicPlayer::MusicPlayer() :
    loaded(false)
    , state(false)
    , current("")
    , _current(0)
    , volume(50)
{
    DebugLog(SH_INFO, "Creating a music player instance");
    this->sounds_name = {
        "01.ogg", "02.ogg", "03.ogg", "04.ogg", "05.ogg", "06.ogg", "07.ogg", "08.ogg", "09.ogg", "10.ogg",
        "11.ogg", "12.ogg"
    };
}

bool MusicPlayer::load()
{
    DebugLog(SH_INFO, "Loading MusicPlayer");

    this->loaded = true;

    return this->loaded;
}

bool MusicPlayer::getState()
{
    return this->state;
}

void MusicPlayer::setVolumeConst(int vol)
{
    this->volume = vol;
}

float MusicPlayer::getVolume()
{
    return this->music.getVolume();
}

void MusicPlayer::mute()
{
    if (this->music.getVolume() > 0.0f)
        this->music.setVolume(0);
    else
        this->music.setVolume(this->volume);
}

bool MusicPlayer::load(const std::string& name)
{
    DebugLog(SH_INFO, "Loading music : assets/sounds/" << name);
    if (!this->music.openFromFile("assets/sounds/" + name))
    {
        DebugLog(SH_ERR, "Could not load assets/sounds/" << name);
        return false;
    }
    DebugLog(SH_INFO, "Setting volume");
    this->music.setVolume(this->volume);

    return true;
}

bool MusicPlayer::play(const std::string& name)
{
    if (!this->loaded)
        DebugLog(SH_WARN, "Need to load MusicPlayer !");

    int index = 0;
    while (this->sounds_name[index] != name)
    {
        if (index == this->sounds_name.size() - 1)
            break;
        index++;
    }

    if (index == this->sounds_name.size() - 1)
        return false;

    this->stop();  // in case we are already playing something
    this->current = name;
    if (!this->load(name))
        return false;
    this->music.play();
    this->state = true;

    return true;
}

void MusicPlayer::stop()
{
    if (!this->loaded)
        DebugLog(SH_WARN, "Need to load MusicPlayer !");

    if (this->state == true)
    {
        DebugLog(SH_INFO, "Stopping music");
        this->state = false;
        this->music.stop();
    }
}

std::string MusicPlayer::getNext()
{
    if (!this->loaded)
        DebugLog(SH_WARN, "Need to load MusicPlayer !");

    int index = 0;
    while (this->sounds_name[index] != this->current)
    {
        if (index == this->sounds_name.size() - 1)
            break;
        index++;
    }
    if (index == this->sounds_name.size() - 1)
        return this->sounds_name[0];
    return this->sounds_name[index + 1];
}

const std::string& MusicPlayer::getCurrentName()
{
    return this->current;
}
