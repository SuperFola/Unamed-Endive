#include <iostream>

#include "musicplayer.hpp"

MusicPlayer::MusicPlayer()
{
    this->sounds_name = {
        "01.ogg", "02.ogg", "03.ogg", "04.ogg", "05.ogg", "06.ogg", "07.ogg", "08.ogg", "09.ogg", "10.ogg",
        "11.ogg", "12.ogg"
    };
}

bool MusicPlayer::load()
{
    std::cout << "Loading musics" << std::endl;

    for (auto fname: this->sounds_name)
    {
        sf::SoundBuffer* buffer;
        if (!buffer->loadFromFile("assets/sounds/" + fname))
            return false;
        this->sdmgr.addSound(fname, buffer);
        this->sounds[fname] = sf::Sound();
        this->sounds[fname].setBuffer(this->sdmgr.getBuffer(fname));
    }

    std::cout << "Loaded musics" << std::endl;

    return true;
}

void MusicPlayer::play(const std::string name)
{
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
}

void MusicPlayer::stop()
{
    if (this->current != "")
    {
        this->sounds[this->current].stop();
        this->current = "";
    }
}

std::string MusicPlayer::getNext()
{
    int index = 0;
    while (this->sounds_name[index] != this->current)
    {
        index++;
    }
    if (index == this->sounds_name.size() - 1)
        return this->sounds_name[0];
    return this->sounds_name[index + 1];
}
