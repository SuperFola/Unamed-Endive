#include <iostream>

#include "pnjmanager.hpp"

PNJManager::PNJManager()
{

}

void PNJManager::add_pnj_on_map(int mid, PNJ pnj, std::string dname)
{
    if (this->pnjs.find(mid) == this->pnjs.end())
    {
        this->pnjs[mid] = std::vector<PNJ>;
    }
    pnj.setDisplayName(dname);
    if (!pnj.load())
        std::cout << "Unable to load " << dname << " PNJ" << std::endl;
    this->pnjs[mid].push_back(pnj);
}

int PNJManager::countPNJonMap(int mid)
{
    if (this->pnjs.find(mid) == this->pnjs.end())
        return 0;
    return this->pnjs[mid].size();
}

PNJ& PNJManager::getPNJonMap(int mid, int pid)
{
    return this->pnjs[mid][pid];
}
