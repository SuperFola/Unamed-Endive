#include "block.hpp"

// public
Block::Block() :
    solid(false)
    , id(-1)
    , reset_trigger(true)
    , trigg_kind(Trigger::NOTHING)
{

}

Block::Block(int id_, bool solid_) :
    solid(solid_)
    , id(id_)
    , reset_trigger(true)
    , trigg_kind(Trigger::NOTHING)
{

}

Block::Block(int id_, bool solid_, bool reset_trig_) :
    solid(solid_)
    , id(id_)
    , reset_trigger(reset_trig_)
    , trigg_kind(Trigger::NOTHING)
{

}

bool Block::is_solid()
{
    return this->solid;
}

int Block::getId()
{
    return this->id;
}

void setTriggerReset(bool value)
{
    this->reset_trigger = value;
}

void add_trigger(const std::string& name, const std::string& action, Trigger kind) // temp
{
    if (this->trigg_kind == Trigger::NOTHING)
    {
        this->trigger.emplace(name, action);
        this->trigg_kind = kind;
    }
    else
        std::cout << "Can not change the kind of the trigger" << std::endl;
}
