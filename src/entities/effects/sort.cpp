#include "sort.hpp"

// public
Sort::Sort() :
    type(SortilegeType::UniqueTargetAdvDamage)
    , damages(0)
    , targets(0)
{

}

bool Sort::load(SortilegeType stype, int dmg, int targets/*, const std::string& name*/)
{
    this->type = stype;
    this->damages = dmg;
    this->targets = targets;
    /*this->name = name;*/

    return true;
}

bool Sort::loadfrom(Sort& other)
{
    this->type = other.type;
    this->damages = other.damages;
    this->targets = other.targets;

    return true;
}

void Sort::act(Creature*)
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

    // unique us
    case SortilegeType::UniqueTargetUsHeal:
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

    // multiple us
    case SortilegeType::MultipleUsHeal:
        break;

    case SortilegeType::MultipleUsHealStatus:
        break;
    }
}

SortilegeType Sort::getType()
{
    return this->type;
}

Json::Value Sort::serialize()
{
    Json::Value value;

    value["type"] = static_cast<int>(this->type);
    value["damages"] = this->damages;
    value["targets"] = this->targets;

    return value;
}
