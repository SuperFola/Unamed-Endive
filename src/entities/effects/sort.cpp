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

int Sort::act(Creature* other)
{
    int r = this->damages;

    switch (this->type)
    {
    case SortilegeType::MultipleAdvDamage:
    case SortilegeType::UniqueTargetAdvDamage:
        break;

    case SortilegeType::MultipleAdvPoison:
    case SortilegeType::UniqueTargetAdvPoison:
        other->setStatus(POISONED);
        break;

    case SortilegeType::MultipleAdvBurn:
    case SortilegeType::UniqueTargetAdvBurn:
        other->setStatus(BURNED);
        break;

    case SortilegeType::MultipleAdvParalize:
    case SortilegeType::UniqueTargetAdvParalize:
        other->setStatus(PARALYSED);
        break;

    case SortilegeType::MultipleUsHeal:
    case SortilegeType::UniqueTargetUsHeal:
        r = 0;
        other->atk_heal_pv(this->damages);
        break;

    case SortilegeType::MultipleUsHealStatus:
        r = 0;
        other->atk_heal_status();
        break;
    }

    return r;
}

SortilegeType Sort::getType()
{
    return this->type;
}

int Sort::getTargets()
{
    return this->targets;
}

Json::Value Sort::serialize()
{
    Json::Value value;

    value["type"] = static_cast<int>(this->type);
    value["damages"] = this->damages;
    value["targets"] = this->targets;

    return value;
}
