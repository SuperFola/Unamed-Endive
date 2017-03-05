#include <iostream>
#include "../../debug.hpp"

#include "pnjmanager.hpp"

PNJManager::PNJManager()
{

}

void PNJManager::add_pnj_on_map(int mid, const std::string& name, const std::string& text, PNJkind kind, const std::string& dname, int x, int y)
{
    PNJ* pnj = new PNJ(name, text, kind, x, y);
    pnj->setDisplayName(dname);
    if (!pnj->load())
        DebugLog(SH_ERR, "Unable to load " << dname << " PNJ");

    if (this->pnjs.find(mid) == this->pnjs.end())
    {
        this->pnjs[mid] = std::vector<PNJ*>();
    }
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
    return *this->pnjs[mid][pid];
}

void PNJManager::update(int mid, sf::RenderWindow& window, sf::Time elapsed)
{
    if (this->pnjs.find(mid) == this->pnjs.end())
        goto _end;

    for (int i=0; i < this->pnjs[mid].size(); i++)
    {
        this->pnjs[mid][i]->update(window, elapsed);
    }
    _end:;
}
