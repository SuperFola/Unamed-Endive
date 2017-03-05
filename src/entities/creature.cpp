#include <iostream>
#include "../../debug.hpp"

#include "creature.hpp"

// public
Creature::Creature() :
    id(0)
    , type(Type::NORMAL)
    , atk(1)
    , def(1)
    , life(DEFAULT_LIFE)
    , max_life(DEFAULT_LIFE)
    , sortilege()
    , state(State::STD)
    , level(0)
{

}

bool Creature::load(int id, Type& t, int atk, int def, int life, int mlife, std::string name, State& s, int level, SortilegeType& stype, int sdmg, int scd, int stargets)
{
    DebugLog(SH_INFO, "Loading creature " << name);
    this->id = id;
    this->type = t;
    this->atk = atk;
    this->def = def;
    this->life = life;
    this->max_life = mlife;
    this->name = name;
    this->state = s;
    this->level = level;
    return this->sortilege.load(stype, sdmg, scd, stargets);
}

bool Creature::loadfrom(Creature* other)
{
    DebugLog(SH_INFO, "Loading creature from another one");
    this->id = other->id;
    this->type = other->type;
    this->atk = other->atk;
    this->def = other->def;
    this->life = other->life;
    this->max_life = other->max_life;
    this->name = other->name;
    this->state = other->state;
    this->level = other->level;
    return this->sortilege.loadfrom(other->sortilege);
}

void Creature::print()
{
    DebugLog(SH_SPE, "id: " << this->id
                                 << "type: " << static_cast<int>(this->type)
                                 << " life: " << this->life
                                 << " max life: " << this->max_life
                                 << " name : " << this->name
                                 << " state : " << static_cast<int>(this->state)
                                 << " level : " << this->level);
}

int Creature::getLife()
{
    return this->life;
}

int Creature::getLevel()
{
    return this->level;
}

std::string& Creature::getName()
{
    return this->name;
}

int Creature::getId()
{
    return this->id;
}

State Creature::getState()
{
    return this->state;
}

Type Creature::getType()
{
    return this->type;
}

int Creature::getAtk()
{
    return this->atk;
}

int Creature::getDef()
{
    return this->def;
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

    value["id"] = this->id;
    value["type"] = static_cast<int>(this->type);
    value["life"] = this->life;
    value["max_life"] = this->max_life;
    value["name"] = this->name;
    value["state"] = static_cast<int>(this->state);
    value["level"] = this->level;
    value["sort"] = this->sortilege.serialize();
    value["atk"] = this->atk;
    value["def"] = this->def;

    return value;
}
