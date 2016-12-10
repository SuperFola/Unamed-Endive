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

int Creature::getLife()
{
    return this->life;
}

void Creature::update(std::vector<Creature>& board)
{
    if (! this->sortilege.getCooldown())
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
