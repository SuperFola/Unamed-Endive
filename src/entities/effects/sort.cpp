#include "sort.hpp"

// public
Sort::Sort()
{

}

int Sort::getCooldown()
{
    return this->current_cooldown;
}

bool Sort::load(SortilegeType stype, int dmg, int cd, int targets)
{
    this->type = stype;
    this->damages = dmg;
    this->cooldown = cd;
    this->current_cooldown = 0;
    this->targets = targets;

    return true;
}

void Sort::update()
{
    // update cooldown
}

void Sort::act(std::vector<Creature>& board)
{

}

Json::Value Sort::serialize()
{
    Json::Value value;

    value["type"] = static_cast<int>(this->type);
    value["damages"] = this->damages;
    value["cooldown"] = this->cooldown;
    value["targets"] = this->targets;

    return value;
}
