#include <iostream>

#include "character.hpp"

// public
Character::Character(Sex sex_) :
    AnimatedEntity(16, 16)
    , name("Someone")
    , sex(sex_)
{
    std::cout << "Character loader" << std::endl;

    if (this->sex == Sex::Male)
        this->path = "assets/players/male/";
    else if (this->sex == Sex::Female)
        this->path = "assets/players/female";
}

void Character::setName(const std::string new_name)
{
    this->name = new_name;
}

void Character::_load()
{
    this->bag.load("saves/" + this->name + "/bag.json");
    this->equip.load("saves/" + this->name + "/equip.json");
    this->dex.load("saves/" + this->name + "/dex.json");

    #ifdef PLATFORM_WIN
    system((std::string("mkdir saves & cd saves & mkdir ") + this->name).data());
    #endif // PLATFORM_WIN
    #ifdef PLATFORM_POSIX
    system("mkdir -p saves/" + this->name.data());
    #endif // PLATFORM_POSIX
}

int Character::save()
{
    // save to "saves/player_name.umd"
    return 0;
}

Bag* Character::getBag()
{
    return &this->bag;
}

Equip* Character::getEquip()
{
    return &this->equip;
}

Dex* Character::getDex()
{
    return &this->dex;
}
