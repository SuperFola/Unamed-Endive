#include "hud.hpp"

// public
Hud::Hud(int id_) :
    id(id_)
    , is_triggered(false)
{

}

void Hud::setTrigger(bool value)
{
    this->is_triggered = value;
}

bool Hud::isTriggered()
{
    return this->is_triggered;
}
