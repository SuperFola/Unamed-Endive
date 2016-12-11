#include <iostream>

#include "equip.hpp"

// public
Equip::Equip() :
    max_crea(3)
{

}

bool Equip::load(const std::string& fpath)
{
    // load file from player save
    std::cout << "Loading equip" << std::endl;

    std::ifstream file(fpath);
    file >> this->root;

    int i;
    for (i=0; i < this->root["creatures"].size(); i++)
    {
        if (!this->root["creatures"][i].get("empty", false).asBool())
        {
            Creature* crea = new Creature();
            int _t = this->root["creatures"][i]["type"].asInt()
                  ,  _s = this->root["creatures"][i]["state"].asInt()
                  , _st = this->root["creatures"][i]["sort"]["type"].asInt()
                  , life = this->root["creatures"][i]["life"].asInt()
                  , mlife = this->root["creatures"][i]["max_life"].asInt()
                  , level = this->root["creatures"][i]["level"].asInt()
                  , sdmg = this->root["creatures"][i]["sort"]["damages"].asInt()
                  , scd = this->root["creatures"][i]["sort"]["cooldown"].asInt()
                  , stargets = this->root["creatures"][i]["sort"]["targets"].asInt();

            Type t = static_cast<Type>(_t % 8);
            State s = static_cast<State>(_s % 4);
            SortilegeType st = static_cast<SortilegeType>(_st % 14);

            crea->load(t, life, mlife, this->root["creatures"][i]["name"].asString(), s, level, st, sdmg, scd, stargets);
            this->equip.push_back(crea);

            std::cout << "Loaded creature " << i << std::endl;
        }
    }
    if (!i)
    {
        // create default creature
        std::cout << "No content found, creating default creature" << std::endl;
    }

    return true;
}

// deprecated
bool Equip::add_creature(Creature& crea)
{
    // this->equip.push_back(crea);
    return true;
}

bool Equip::remove_creature(int id)
{
    if (0 <= id && id < this->max_crea)
    {
        pop<Creature*>(&(this->equip), id);
        return true;
    }
    std::cout << "The creature id in the equip is not correct" << std::endl;
    return false;
}

Creature* Equip::getCrea(int id)
{
    if (0 <= id && id < this->max_crea)
    {
        return this->equip[id];
    }
    std::cout << "The creature id in the equip is not correct" << std::endl;
}

void Equip::increase_size()
{
    this->max_crea++;
}

int Equip::getSize()
{
    return this->max_crea;
}

void Equip::save(const std::string& path)
{
    Json::Value value;
    value["creatures"] = Json::Value();

    int i;
    for (i=0; i < this->equip.size(); i++)
    {
        value["creatures"].append(this->equip[i]->serialize());
    }
    if (!i)
    {
        Json::Value content;
        content["empty"] = true;
        value["creatures"].append(content);
    }

    std::ofstream output(path);
    output << value;
}
