#include <iostream>
#include <fstream>
#include "../../debug.hpp"

#include "dex.hpp"

Dex::Dex() :
    _current(0)
{

}

bool Dex::load(const std::string& path)
{
    DebugLog(SH_INFO, "Loading dex");

    std::ifstream file("assets/config/dex.json");
    file >> this->root;
    bool player_save = false;
    if (is_file_existing(path))
    {
        DebugLog(SH_OK, "Found the player dex");
        std::ifstream player(path);
        player >> this->root_player;
        player_save = true;
    }
    else
        DebugLog(SH_WARN, "No dex found for the player");

    int _id = 0;
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
        this->names.push_back(key.asString());
        this->id_name[_id] = key.asString();

        _id++;
    }

    struct DexInfo default_inf;
    default_inf.viewed = false;
    default_inf.captured = false;
    default_inf.type = Type::NORMAL;
    default_inf.stade = 99;
    default_inf.evolution = "";
    default_inf.file = "default.png";

    this->content["default"] = default_inf;
    this->id_name[999] = "default";

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

std::string Dex::getNext()
{
    if (this->_current != this->names.size())
        return this->names[this->_current++];
    this->_current = 0;
    return "";
}

DexInfo Dex::getInfo(const std::string& name)
{
    if (this->content.find(name) != this->content.end())
        return this->content[name];
    DebugLog(SH_ERR, "Can not find creature with name '" << name << "', returning a default one");

    return this->content["default"];
}

DexInfo Dex::getInfo(int id)
{
    if (this->id_name.find(id) != this->id_name.end())
        return this->content[this->id_name[id]];
    DebugLog(SH_ERR, "Can not find creature with id " << id << ", returning a default one");

    return this->content["default"];
}

void Dex::register_viewed(int id)
{
    if (this->id_name.find(id) != this->id_name.end())
        this->content[this->id_name[id]].viewed = true;
    DebugLog(SH_ERR, "Can not find creature with id " << id << " while trying to set it as viewed");
}

void Dex::register_capture(int id)
{
    if (this->id_name.find(id) != this->id_name.end())
        this->content[this->id_name[id]].captured = true;
    DebugLog(SH_ERR, "Can not find creature with id " << id << " while trying to set it as captured");
}
