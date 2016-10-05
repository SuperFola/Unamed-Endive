#include <iostream>
#include "bag.hpp"

Bag::Bag()
{

}

void Bag::add_pocket(Pocket pocket)
{
    this->pockets.push_back(pocket);
}

Pocket* Bag::getPocket(int id)
{
    if (0 <= id && id < this->pockets.size())
        return &this->pockets[id];
    else
    {
        std::cout << "Can not find the pocket with the id " << id << std::endl;
        return &this->pockets[0]; // return default one to avoid problems
    }
}
