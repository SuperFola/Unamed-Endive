#include <iostream>

#include "creature.hpp"

// public
Creature::Creature() :
    type(Type::NORMAL)
    , life(DEFAULT_LIFE)
    , max_life(DEFAULT_LIFE)
    , sortilege()
    , state(State::STD)
    , level(0)
{

}

bool Creature::load(Type& t, int life, int mlife, std::string name, State& s, int level, SortilegeType& stype, int sdmg, int scd, int stargets)
{
    std::cout << "Loading creature '" << name << "'" << std::endl;
    this->type = t;
    this->life = life;
    this->max_life = mlife;
    this->name = name;
    this->state = s;
    this->level = level;
    return this->sortilege.load(stype, sdmg, scd, stargets);
}

void Creature::print()
{
    std::cout << "type: "
                    << static_cast<int>(this->type)
                    << " life: "
                    << this->life
                    << " max life: "
                    << this->max_life
                    << " name : "
                    << this->name
                    << " state : "
                    << static_cast<int>(this->state)
                    << " level : "
                    << this->level
                    << std::endl;
}

int Creature::getLife()
{
    return this->life;
}

void Creature::update(std::vector<Creature>& board)
{
    if (!this->sortilege.getCooldown())
        // attack
        this->sortilege.act(board);
    else
        ;
}

Json::Value Creature::serialize()
{
    Json::Value value;

    value["type"] = static_cast<int>(this->type);
    value["life"] = this->life;
    value["max_life"] = this->max_life;
    value["name"] = this->name;
    value["state"] = static_cast<int>(this->state);
    value["level"] = this->level;
    value["sort"] = this->sortilege.serialize();

    return value;
}
