#include "creature.hpp"

// public
Creature::Creature() :
    type(Type::NORMAL)
    , life(DEFAULT_LIFE)
    , max_life(DEFAULT_LIFE)
    , id(0)
    , revealed(false)
    , sortilege()
    , state(State::NORMAL)
    , level(0)
{

}

Creature::Creature(int given_id) :
    type(Type::NORMAL)
    , life(DEFAULT_LIFE)
    , max_life(DEFAULT_LIFE)
    , id(given_id)
    , revealed(false)
    , sortilege()
    , state(State::NORMAL)
    , level(0)
{

}

Creature::Creature(int given_id, bool given_revealed) :
    type(Type::NORMAL)
    , life(DEFAULT_LIFE)
    , max_life(DEFAULT_LIFE)
    , id(given_id)
    , revealed(given_revealed)
    , sortilege()
    , state(State::NORMAL)
    , level(0)
{

}

void Creature::reveal()
{
    this->revealed = true;
}

bool Creature::is_revealed()
{
    return this->revealed;
}

int Creature::getLife()
{
    return this->life;
}

// private
