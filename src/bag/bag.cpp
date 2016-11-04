#include <iostream>
#include <fstream>

#include "bag.hpp"
#include "../abstract/functions.hpp"

Bag::Bag()
{

}

bool Bag::load(const std::string& path)
{
    std::cout << "Loading bag" << std::endl;

    const std::vector<std::string> pockets = {
        "pocket1",
        "pocket2",
        "pocket3",
        "pocket4",
        "pocket5"
    };

    if (!is_file_existing(path))
    {
        std::cout << "Can not find the file, creating a default bag" << std::endl;
        // we will create a bag by default
        for (const auto& pname: pockets)
        {
            Pocket* pocket = new Pocket(pname, true);
            this->add_pocket(pocket);
        }

        std::cout << "Bag loaded" << std::endl;
        return true;
    }

    std::ifstream file(path);
    file >> this->root;

    for (const auto& pname: pockets)
    {
        Pocket* pocket = new Pocket(pname);
        if (!pocket->load(this->root[pname]))
                return false;
        this->add_pocket(pocket);
    }

    std::cout << "Bag loaded" << std::endl;

    return true;
}

void Bag::add_pocket(Pocket* pocket)
{
    this->pockets.push_back(pocket);
}

Pocket* Bag::getPocket(int id)
{
    if (0 <= id && id < this->pockets.size())
        return this->pockets[id];
    else
    {
        std::cout << "Can not find the pocket with the id " << id << std::endl;
        return this->pockets[0]; // return default one to avoid problems
    }
}

void Bag::serialize(Json::Value& value)
{
    // modify the reference to the value
    for (int i=0; i < this->pockets.size(); i++)
    {
        value.append(this->pockets[i]->serialize());
    }
}
