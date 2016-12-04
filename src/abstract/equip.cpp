#include <iostream>

#include "equip.hpp"
#include "functions.hpp"

// public
Equip::Equip() :
    max_crea(3)
{

}

bool Equip::load(const std::string& file)
{
    // load file from player save
}

bool Equip::add_creature(Creature& crea)
{
    this->equip.push_back(crea);
    return true;
}

bool Equip::remove_creature(int id)
{
    if (0 <= id && id < this->max_crea)
    {
        pop<Creature>(&(this->equip), id);
        return true;
    }
    std::cout << "The creature id in the equip is not correct" << std::endl;
    return false;
}

Creature& Equip::getCrea(int id)
{
    if (0 <= id && id < this->max_crea)
        return this->equip[id];
    std::cout << "The creature id in the equip is not correct" << std::endl;
}

void Equip::increase_size()
{
    this->max_crea++;
}

int Equip::getSize()
{
    return this->max_crea;
}

// private
