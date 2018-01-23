#include <iostream>
#include "block.hpp"

// public
Block::Block() :
    solid(false)
    , id(-1)
{

}

Block::Block(int id_, bool solid_) :
    solid(solid_)
    , id(id_)
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

void Block::setId(int newone)
{
    this->id = newone;
}

void Block::setSolid(bool value)
{
    this->solid = value;
}
