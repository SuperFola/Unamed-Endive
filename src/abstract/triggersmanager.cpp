#include <iostream>
#include "triggersmanager.hpp"

TriggersManager::TriggersManager()
{

}

void TriggersManager::add_trigger(int mid, int rid, std::string identifier)
{
    if (this->triggs.find(mid) != this->triggs.end())
    {
        this->triggs[mid][rid] = identifier;
    }
    else
    {
        std::map<int, std::string> temp;
        temp.emplace(rid, identifier);
        this->triggs.emplace(mid, temp);
    }
}

std::string TriggersManager::get_trigg(int mid, int rid)
{
    if (this->triggs.find(mid) != this->triggs.end())
    {
        return this->triggs[mid][rid];
    }
    return this->notrigger;
}

bool TriggersManager::is_notrigger(const std::string& str)
{
    return this->notrigger.compare(str) != 0;
}
