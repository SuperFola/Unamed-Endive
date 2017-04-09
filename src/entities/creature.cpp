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
    , pp(DEFAULT_PP)
    , max_pp(DEFAULT_PP)
    , exp(0)
{

}

bool Creature::load(int id, Type& t, int atk, int def, int life, int mlife, int pp, int mpp, std::string name, State& s, int level, int exp, SortilegeType& stype, int sdmg, int scd, int stargets)
{
    DebugLog(SH_INFO, "Loading creature " << name);
    this->id = id;
    this->type = t;
    this->atk = atk;
    this->def = def;
    this->life = life;
    this->max_life = mlife;
    this->pp = pp;
    this->max_pp = mpp;
    this->name = name;
    this->state = s;
    this->level = level;
    this->exp = exp;
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
    this->pp = other->pp;
    this->max_pp = other->max_pp;
    this->exp = other->exp;
    return this->sortilege.loadfrom(other->sortilege);
}

void Creature::print()
{
    DebugLog(SH_SPE, "id : " << this->id
                                 << "type : " << static_cast<int>(this->type)
                                 << " life: " << this->life
                                 << " max life: " << this->max_life
                                 << " name : " << this->name
                                 << " state : " << static_cast<int>(this->state)
                                 << " level : " << this->level
                                 << " pp : " << this->pp
                                 << " max pp : " << this->max_pp
                                 << " exp : " << this->exp);
}

int Creature::getLife()
{
    return this->life;
}

int Creature::getMaxLife()
{
    return this->max_life;
}

int Creature::getLevel()
{
    return this->level;
}

int Creature::getPP()
{
    return this->pp;
}

int Creature::getMaxPP()
{
    return this->max_pp;
}

int Creature::getExp()
{
    return this->exp;
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
    value["pp"] = this->pp;
    value["max_pp"] = this->max_pp;
    value["exp"] = this->exp;

    return value;
}

bool Creature::healPV(int value)
{
    bool r = false;

    switch (value)
    {
    case -1:
        if (this->life > 0 && this->life != this->max_life)
        {
            this->life = this->max_life;
            r = true;
        }
        break;

    case -2:
        if (this->life == 0)
        {
            this->life = this->max_life;
            r = true;
        }
        break;

    case -3:
        if (this->life == 0)
        {
            this->life = this->max_life / 2;
        }
        break;

    default:
        if (this->life > 0 && this->life < this->max_life && value > 0)
        {
            this->life += value;
            if (this->life > this->max_life)
                this->life = this->max_life;
            r = true;
        }
        break;
    }

    return r;
}

bool Creature::healPP(int value)
{
    bool r = false;

    switch (value)
    {
    case -1:
        if (this->pp != this->max_pp)
        {
            this->pp = this->max_pp;
            r = true;
        }
        break;

    default:
        if (this->pp < this->max_pp && value > 0)
        {
            this->pp += value;
            if (this->pp > this->max_pp)
                this->pp = this->max_pp;
            r = true;
        }
        break;
    }

    return r;
}

bool Creature::healStatus(int value)
{
    bool r = false;

    switch (value)
    {
    case 0:
        if (this->state == State::BURNED)
        {
            this->state = State::STD;
            r = true;
        }
        break;

    case 1:
        if (this->state == State::PARALYSED)
        {
            this->state = State::STD;
            r = true;
        }
        break;

    case 2:
        if (this->state == State::POISONED)
        {
            this->state = State::STD;
            r = true;
        }
        break;

    default:
        if (this->state != State::STD)
        {
            this->state = State::STD;
            r = true;
        }
        break;
    }

    return r;
}

bool Creature::levelUP(int value)
{
    this->level += value;
    this->exp = 0;

    return true;
}

bool Creature::lowercooldown(int value)
{
    return this->sortilege.lowercooldown(value);
}
