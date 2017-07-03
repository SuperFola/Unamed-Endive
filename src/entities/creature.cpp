#include <iostream>
#include "../../debug.hpp"
#include <cstdlib>

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

bool Creature::load(int id, Type& t, int atk, int def, int life, int mlife, int pp, int mpp, std::string name, State s, int level, long int exp, SortilegeType& stype, int sdmg, int stargets)
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
    return this->sortilege.load(stype, sdmg, stargets);
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

int Creature::calculateLevelFromXp(long int exp)
{
    return int(pow(exp, 0.33));
}

long int Creature::calculateExpFromLevel(int level)
{
    return int(pow(level, 3));
}

int Creature::calculatePPFromLevel(int level)
{
    return int(floor(pow(level, 0.5) + (level / 12.0f) + 3));
}

int Creature::calculateStatFromLevel(int level)
{
    return int(pow(level, 0.5) * 10);
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

long int Creature::getExp()
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

std::string Creature::getStringState()
{
    std::string s = "none";
    switch (this->state)
    {
    case BURNED:
        s = "BRU";
        break;

    case PARALYSED:
        s = "PAR";
        break;

    case POISONED:
        s = "PSN";
        break;

    case STD:
        break;
    }
    return s;
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
    value["exp"] = double(this->exp);

    return value;
}

int Creature::gainExp(Creature* other)
{
    int old = this->level;

    this->exp += int((1.0f / float(other->level)) * other->exp) + 5;
    other->exp -= int((1.0f / float(other->level)) * other->exp) + 5;
    other->level = Creature::calculateLevelFromXp(other->exp);

    this->level = Creature::calculateLevelFromXp(this->exp);

    return this->level - old;
}

void Creature::attack(Creature* other)
{
    float amplifier = TypesTable::atktype_on_deftype(this->type, other->type);
    int out = this->sortilege.act(other);
    DebugLog(SH_OK, this->name << " : " << "x" << amplifier << ", dmg: " << out);
    if (out != 0)
    {
        float real_damages = (0.85f + (rand() % 16) / 100.0f) * amplifier * ((((((this->level + 1) * 0.4f + 2) * out * this->atk) / 50.0f) / other->def) + 2);
        other->life -= real_damages;
        if (other->life < 0)
        {
            other->life = 0;
            other->state = STD;
        }
        DebugLog(SH_OK, "      applied: " << real_damages << ", atk: " << this->atk << ", (other) def: " << other->def);
    }
}

Sort* Creature::getSort()
{
    return &this->sortilege;
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
    this->exp = Creature::calculateExpFromLevel(this->level);

    return true;
}

void Creature::setStatus(State s)
{
    this->state = s;
}

void Creature::atk_heal_pv(int pv)
{
    this->life += pv;
    if (this->life > this->max_life)
        this->life = this->max_life;
}

void Creature::atk_heal_status()
{
    this->state = State::STD;
}

int Creature::updateState()
{
    int r = 0;
    switch (this->state)
    {
    case BURNED:
        this->life -= int(this->max_life / 10.0f);
        if (this->life < 0)
        {
            this->life = 0;
            this->state = STD;
            r = DEAD_BUR;
        }
        else
        {
            if ((rand() % 101) <= 30)
                this->state = STD;
            r = LOST_BUR;
        }
        break;

    case POISONED:
        this->life -= int(this->max_life / 8.0f);
        if (this->life < 0)
        {
            this->life = 0;
            this->state = STD;
            r = DEAD_PSN;
        }
        else
        {
            if ((rand() % 101) <= 40)
            {
                this->state = STD;
                r = LOST_PSN;
            }
        }
        break;

    case PARALYSED:
        if ((rand() % 101) <= 45)
            {
                this->state = STD;
                r = LOST_PAR;
            }
        break;

    default:
        break;
    }

    return r;
}

