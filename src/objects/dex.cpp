#include <iostream>
#include <fstream>

#include "dex.hpp"

Dex::Dex()
{

}

bool Dex::load(const std::string& path)
{
    std::cout << "Loading dex" << std::endl;

    std::ifstream file("assets/config/dex.json");
    file >> this->root;
    bool player_save = false;
    if (is_file_existing(path))
    {
        std::ifstream player(path);
        player >> this->root_player;
        player_save = true;
    }

    for (Json::ValueIterator itr = root.begin() ; itr != root.end() ; itr++)
    {
        struct DexInfo* dexi;
        Json::Value key = itr.key();
        Json::Value value = *itr;

        if (!player_save)
        {
            dexi->viewed = false;
            dexi->captured = false;
        }
        else
        {
            if (root_player.get(key.asString(), -1) != -1)
            {
                dexi->viewed = root_player[key.asString()]["viewed"].asBool();
                dexi->captured = root_player[key.asString()]["captured"].asBool();
            }
            else
            {
                dexi->viewed = false;
                dexi->captured = false;
            }
        }
        switch (value["type"].asInt())
        {
        case 0:
            dexi->type = Type::NORMAL;
            break;

        case 1:
            dexi->type = Type::FIRE;
            break;

        case 2:
            dexi->type = Type::WATER;
            break;

        case 3:
            dexi->type = Type::GRASS;
            break;

        case 4:
            dexi->type = Type::FLYING;
            break;

        case 5:
            dexi->type = Type::FIGHTING;
            break;

        case 6:
            dexi->type = Type::POISON;

        default:
            dexi->type = Type::NORMAL;
            break;
        }
        dexi->stade = value["stade"].asInt();
        dexi->evolution = value["evolution"].asString();

        this->content[key.asString()] = dexi;
    }

    return true;
}
