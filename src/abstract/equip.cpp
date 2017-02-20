#include <iostream>

#include "equip.hpp"

// public
Equip::Equip() :
    max_crea(3)
    , pc(false)
{

}

bool Equip::load(const std::string& fpath, bool pc)
{
    this->pc = pc;

    // load file from player save
    if (!this->pc)
        std::cout << "Loading equip" << std::endl;
    else
        std::cout << "Loading pc" << std::endl;

    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> distlife(14, 22);

    if (!is_file_existing(fpath))
    {
        if (this->pc)
            goto dont_create_for_pc;

        {
            // create default creature
            std::cout << "No content found, creating default creature" << std::endl;
            Creature* crea = new Creature();
            int  id = 0 // accologion
                  , _t = 0  // Type::NORMAL
                  ,  _s = 3  // State::STD
                  , _st = 0  // SortilegeType::UniqueTargetAdvDamage
                  , life = int(distlife(rng))  // mlife = life
                  , level = 1
                  , sdmg = 5  // damages for the sortilege
                  , scd = 4  // cooldown for the sortilege
                  , stargets = 1  // targets for the sortilege
                  , atk = 10  // attack of the creature
                  , def = 10;  // defense

            Type t = static_cast<Type>(_t % 8);
            State s = static_cast<State>(_s % 4);
            SortilegeType st = static_cast<SortilegeType>(_st % 14);

            crea->load(id, t, atk, def, life, life, "first creature", s, level, st, sdmg, scd, stargets);
            this->equip.push_back(crea);
        }

        dont_create_for_pc:;
    }
    else
    {
        std::ifstream file(fpath);
        file >> this->root;

        int i;
        for (i=0; i < this->root["creatures"].size(); i++)
        {
            if (!this->root["creatures"][i].get("empty", false).asBool())
            {
                Creature* crea = new Creature();
                int id = this->root["creatures"][i]["id"].asInt()
                      , _t = this->root["creatures"][i]["type"].asInt()
                      ,  _s = this->root["creatures"][i]["state"].asInt()
                      , _st = this->root["creatures"][i]["sort"]["type"].asInt()
                      , life = this->root["creatures"][i]["life"].asInt()
                      , mlife = this->root["creatures"][i]["max_life"].asInt()
                      , level = this->root["creatures"][i]["level"].asInt()
                      , sdmg = this->root["creatures"][i]["sort"]["damages"].asInt()
                      , scd = this->root["creatures"][i]["sort"]["cooldown"].asInt()
                      , stargets = this->root["creatures"][i]["sort"]["targets"].asInt()
                      , atk = this->root["creatures"][i]["atk"].asInt()
                      , def = this->root["creatures"][i]["def"].asInt();

                Type t = static_cast<Type>(_t % 8);
                State s = static_cast<State>(_s % 4);
                SortilegeType st = static_cast<SortilegeType>(_st % 14);

                crea->load(id, t, atk, def, life, mlife, this->root["creatures"][i]["name"].asString(), s, level, st, sdmg, scd, stargets);
                this->equip.push_back(crea);

                std::cout << "Loaded creature " << i << std::endl;
            }
        }
    }

    return true;
}

// deprecated
bool Equip::add_creature(Creature& crea)
{
    // this->equip.push_back(crea);
    return true;
}

bool Equip::remove_creature(int pos)
{
    if (0 <= pos && pos < this->max_crea)
    {
        pop<Creature*>(&(this->equip), pos);
        return true;
    }
    if (!this->pc)
        std::cout << "The creature's position in the equip is not correct" << std::endl;
    else
        std::cout << "The creature's position in the pc is not correct" << std::endl;
    return false;
}

Creature* Equip::getCrea(int pos)
{
    if (0 <= pos && pos < this->equip.size())
    {
        return this->equip[pos];
    }
    if (!this->pc)
        std::cout << "The creature's position in the equip is not correct" << std::endl;
    else
        std::cout << "The creature's position in the equip is not correct" << std::endl;
}

void Equip::increase_size()
{
    this->max_crea++;
}

int Equip::getSize()
{
    return this->equip.size();
}

void Equip::save(const std::string& path)
{
    Json::Value value;
    value["creatures"] = Json::Value();

    int i = 0;
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
