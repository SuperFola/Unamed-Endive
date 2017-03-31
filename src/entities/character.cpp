#include <iostream>
#include "../../debug.hpp"

#include "character.hpp"
#include "../scripting/scripting.hpp"

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

bool Character::pass_pnj(Map& map_, std::vector<float> vect)
{
    if (this->pnjm->find_pnjid_at(vect[0] + this->pos.getX()                             , vect[1] + this->pos.getY()                             , map_.getId()) != -1 ||
         this->pnjm->find_pnjid_at(vect[0] + this->pos.getX() + 2 * TILE_SIZE, vect[1] + this->pos.getY()                             , map_.getId()) != -1 ||
         this->pnjm->find_pnjid_at(vect[0] + this->pos.getX()                             , vect[1] + this->pos.getY() + 2 * TILE_SIZE, map_.getId()) != -1 ||
         this->pnjm->find_pnjid_at(vect[0] + this->pos.getX() + 2 * TILE_SIZE, vect[1] + this->pos.getY() + 2 * TILE_SIZE, map_.getId()) != -1)
    {
        return false;
    }
    return true;
}

void Character::chara_send_player_touch(Map& map_)
{
    // we must check if we changed case
    if (this->has_changed_case())
    {
        PyScripting::run_code(("trigger_event(" +
                                   to_string<int>(map_.getId()) + "," +
                                   to_string<int>(this->pos.getX() / TILE_SIZE) + "," +
                                   to_string<int>(this->pos.getY() / TILE_SIZE) + "," +
                                   "'player touch')").c_str());
    }
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
    PyScripting::run_code(("if not os.path.exists(\"saves/" + this->name + "\"): os.mkdir(\"saves/" + this->name + "\")").c_str());

    this->bag.load("saves/" + this->name + "/bag.json");
    this->equip.load("saves/" + this->name + "/equip.json");
    this->pc.load("saves/" + this->name + "/pc.json", true);
    this->dex.load("saves/" + this->name + "/dex.json");

    if (is_file_existing("saves/" + this->name + "/player.json"))
    {
        std::ifstream file("saves/" + this->name + "/player.json");
        Json::Value root;
        file >> root;

        this->pos.set(root["pos"]["x"].asFloat(), root["pos"]["y"].asFloat());
        this->sex = static_cast<Sex>(root["sex"].asInt());
        this->path = root["asset"].asString();
    }

}

int Character::save()
{
    // save to "saves/player_name/"
    this->bag.save("saves/" + this->name + "/bag.json");
    this->equip.save("saves/" + this->name + "/equip.json");
    this->pc.save("saves/" + this->name + "/pc.json");
    this->dex.save("saves/" + this->name + "/dex.json");

    Json::Value value;
    Json::Value pos;
    pos["x"] = this->getPos().getX();
    pos["y"] = this->getPos().getY();
    value["sex"] = static_cast<int>(this->sex);
    value["asset"] = this->path;
    value["pos"] = pos;

    std::ofstream output("saves/" + this->name + "/player.json");
    output << value;

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

Equip* Character::getPC()
{
    return &this->pc;
}

Dex* Character::getDex()
{
    return &this->dex;
}

void Character::setPNJm(PNJManager* _pnjm)
{
    this->pnjm = _pnjm;
    DebugLog(SH_INFO, "Adding pnj manager to character");
}
