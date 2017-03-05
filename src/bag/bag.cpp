#include <iostream>
#include <fstream>
#include "../../debug.hpp"

#include "bag.hpp"

Bag::Bag()
{
    this->_pockets = {
        "pocket1",
        "pocket2",
        "pocket3",
        "pocket4",
        "pocket5"
    };
}

bool Bag::load(const std::string& path)
{
    DebugLog(SH_INFO, "Loading bag");

    if (!is_file_existing(path))
    {
        DebugLog(SH_WARN, "Can not find the file, creating a default bag");
        // we will create a bag by default
        for (const auto& pname: this->_pockets)
        {
            Pocket* pocket = new Pocket(pname, true);
            this->pockets.push_back(pocket);
        }

        DebugLog(SH_OK, "Bag loaded");
        return true;
    }

    std::ifstream file(path);
    file >> this->root;

    for (const auto& pname: this->_pockets)
    {
        Pocket* pocket = new Pocket(pname);
        if (!pocket->load(this->root[pname]))
                return false;
        this->pockets.push_back(pocket);
        DebugLog(SH_OK, "Loaded " << pname);
    }

    DebugLog(SH_OK, "Bag loaded");

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
        DebugLog(SH_ERR, "Can not find the pocket with the id " << id);
        return this->pockets[0]; // return default one to avoid problems
    }
}

void Bag::save(const std::string& path)
{
    Json::Value value;

    for (int i=0; i < this->pockets.size(); i++)
    {
        value[this->_pockets[i]] = this->pockets[i]->serialize();
    }

    std::ofstream output(path);
    output << value;
}
