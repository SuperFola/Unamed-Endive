#include <iostream>

#include "musicplayer.hpp"

MusicPlayer::MusicPlayer() :
    loaded(false)
    , state(false)
    , current("")
    , _current(0)
{
    std::cout << "Creating a MusicPlayer instance" << std::endl;
    this->sounds_name = {
        "01.ogg", "02.ogg", "03.ogg", "04.ogg", "05.ogg", "06.ogg", "07.ogg", "08.ogg", "09.ogg", "10.ogg",
        "11.ogg", "12.ogg"
    };
}

bool MusicPlayer::load()
{
    std::cout << "Loading " << this->sounds_name[this->_current] << std::endl;

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("assets/sounds/" + this->sounds_name[this->_current]))
        return false;
    this->sdmgr.addSound(this->sounds_name[this->_current], buffer);
    this->sounds[this->sounds_name[this->_current]] = sf::Sound();
    this->sounds[this->sounds_name[this->_current]].setBuffer(this->sdmgr.getBuffer(this->sounds_name[this->_current]));

    this->_current++;

    if (this->_current == this->sounds_name.size())
        this->loaded = true;

    return this->loaded;
}

bool MusicPlayer::getState()
{
    return this->state;
}

bool MusicPlayer::play(const std::string& name)
{
    if (!this->loaded)
        std::cout << "Need to load MusicPlayer !" << std::endl;

    if (this->sounds.find(name) == this->sounds.end())
        return false;

    this->state = true;

    if (this->current != "")
    {
        // we are already playing something
        if (this->sounds[this->current].getStatus() == sf::SoundSource::Status::Playing)
        {

        }
        else if (this->sounds[this->current].getStatus() == sf::SoundSource::Status::Paused)
        {
            this->sounds[this->current].stop();
        }
    }
    this->current = name;
    this->sounds[name].play();

    return true;
}

void MusicPlayer::stop()
{
    if (!this->loaded)
        std::cout << "Need to load MusicPlayer !" << std::endl;

    if (this->current != "")
    {
        this->state = false;
        this->sounds[this->current].stop();
        this->current = "";
    }
}

std::string MusicPlayer::getNext()
{
    if (!this->loaded)
        std::cout << "Need to load MusicPlayer !" << std::endl;

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
