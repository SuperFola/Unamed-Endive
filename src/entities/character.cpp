#include <iostream>
#include "../../debug.hpp"

#include "character.hpp"

// public
Character::Character(Sex sex_) :
    AnimatedEntity(16, 16)
    , name("Someone")
    , sex(sex_)
{
    DebugLog(SH_INFO, "Character cstr");

    if (this->sex == Sex::Male)
        this->path = "assets/players/male/";
    else if (this->sex == Sex::Female)
        this->path = "assets/players/female";
}

int Character::chara_move(Map& map_, std::vector<float> vect)
{
    // checking all edges and returning the first one to be like != -1
    // which means that it is on a tp, and we got a new rpos
    // so we return it
    int nrp1 = map_.post_colliding_test_to_check_tp(vect[0] / TILE_SIZE + this->pos.getX() / TILE_SIZE,       vect[1] / TILE_SIZE + this->pos.getY() / TILE_SIZE      );
    if (nrp1 != -1) return nrp1;

    int nrp2 = map_.post_colliding_test_to_check_tp(vect[0] / TILE_SIZE + this->pos.getX() / TILE_SIZE + 2, vect[1] / TILE_SIZE + this->pos.getY() / TILE_SIZE      );
    if (nrp2 != -1) return nrp2;

    int nrp3 = map_.post_colliding_test_to_check_tp(vect[0] / TILE_SIZE + this->pos.getX() / TILE_SIZE,       vect[1] / TILE_SIZE + this->pos.getY() / TILE_SIZE + 2);
    if (nrp3 != -1) return nrp3;

    int nrp4 = map_.post_colliding_test_to_check_tp(vect[0] / TILE_SIZE + this->pos.getX() / TILE_SIZE + 2, vect[1] / TILE_SIZE + this->pos.getY() / TILE_SIZE + 2);
    if (nrp4 != -1) return nrp4;

    // nothing was != -1 so we return -1 to tell that we are not on some tp
    return -1;
}

void Character::setName(const std::string new_name)
{
    this->name = new_name;
}

std::string Character::getName()
{
    return this->name;
}

void Character::_load()
{
    DebugLog(SH_INFO, "creating \"saves/" << this->name << "\"");
    #ifdef PLATFORM_WIN
        system((std::string("mkdir saves & cd saves & mkdir \"") + this->name + "\"").c_str());
    #endif // PLATFORM_WIN
    #ifdef PLATFORM_POSIX
        system((std::string("mkdir -p \"saves/") + this->name + "\"").c_str());
    #endif // PLATFORM_POSIX

    this->bag.load("saves/" + this->name + "/bag.json");
    this->equip.load("saves/" + this->name + "/equip.json");
    this->pc.load("saves/" + this->name + "/pc.json", true);
    this->dex.load("saves/" + this->name + "/dex.json");
}

int Character::save()
{
    // save to "saves/player_name/"
    this->bag.save("saves/" + this->name + "/bag.json");
    this->equip.save("saves/" + this->name + "/equip.json");
    this->pc.save("saves/" + this->name + "/pc.json");
    this->dex.save("saves/" + this->name + "/dex.json");

    return 0;
}

void Character::speak(int mid, PNJManager* pnjm)
{
    DebugLog(SH_WARN, "Speaking. Please change the behavior of this method ASAP");
    pnjm->getPNJonMap(mid, 0).speak();
}

Bag* Character::getBag()
{
    return &this->bag;
}

Equip* Character::getEquip()
{
    return &this->equip;
}

Equip* Character::getPC()
{
    return &this->pc;
}

Dex* Character::getDex()
{
    return &this->dex;
}
