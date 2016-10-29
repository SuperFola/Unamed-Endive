#include "hud.hpp"

// public
Hud::Hud(int id_) :
    id(id_)
    , is_trigered(false)
{

}

bool Hud::setTrigger(bool value)
{
    this->is_trigered = value;
}
