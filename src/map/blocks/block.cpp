#include "block.hpp"

// public
Block::Block() :
    solid(false)
    , id(-1)
{

}

Block::Block(int id_) :
    solid(false)
    , id(id_)
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
