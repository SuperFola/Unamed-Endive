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

bool Sort::loadfrom(Sort& other)
{
    this->type = other.type;
    this->damages = other.damages;
    this->cooldown = other.cooldown;
    this->current_cooldown = other.current_cooldown;
    this->targets = other.targets;

    return true;
}

void Sort::update()
{
    // update cooldown
}

void Sort::act(std::vector<Creature>& board)
{
    switch (this->type)
    {
    // unique adv
    case SortilegeType::UniqueTargetAdvDamage:
        break;

    case SortilegeType::UniqueTargetAdvPoison:
        break;

    case SortilegeType::UniqueTargetAdvBurn:
        break;

    case SortilegeType::UniqueTargetAdvParalize:
        break;

    case SortilegeType::UniqueTargetAdvNerfCooldown:
        break;

    // unique us
    case SortilegeType::UniqueTargetUsHeal:
        break;

    case SortilegeType::UniqueTargetUsBuffCooldown:
        break;

    // multiple adv
    case SortilegeType::MultipleAdvDamage:
        break;

    case SortilegeType::MultipleAdvPoison:
        break;

    case SortilegeType::MultipleAdvBurn:
        break;

    case SortilegeType::MultipleAdvParalize:
        break;

    case SortilegeType::MultipleAdvNerfCooldown:
        break;

    // multiple us
    case SortilegeType::MultipleUsHeal:
        break;

    case SortilegeType::MultipleUsBuffCooldown:
        break;
    }
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
