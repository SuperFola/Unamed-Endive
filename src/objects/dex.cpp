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
        std::cout << "Found the player dex" << std::endl;
        std::ifstream player(path);
        player >> this->root_player;
        player_save = true;
    }

    for (Json::ValueIterator itr = root.begin() ; itr != root.end() ; itr++)
    {
        struct DexInfo dexi;
        Json::Value key = itr.key();
        Json::Value value = *itr;

        if (!player_save)
        {
            dexi.viewed = false;
            dexi.captured = false;
        }
        else
        {
            if (root_player.get(key.asString(), -1) != -1)
            {
                dexi.viewed = root_player[key.asString()]["viewed"].asBool();
                dexi.captured = root_player[key.asString()]["captured"].asBool();
            }
            else
            {
                dexi.viewed = false;
                dexi.captured = false;
            }
        }
        dexi.type = static_cast<Type>(value["type"].asInt() % 8);
        dexi.stade = value["stade"].asInt();
        dexi.evolution = value["evolution"].asString();
        dexi.file = value["file"].asString();

        this->content[key.asString()] = dexi;
    }

    return true;
}

void Dex::save(const std::string& path)
{
    Json::Value value;

    for (auto& kv: this->content)
    {
        Json::Value content;
        content["viewed"] = kv.second.viewed;
        content["captured"] = kv.second.captured;
        value[kv.first] = content;
    }

    std::ofstream output(path);
    output << value;
}
