#ifndef DEF_CONTAINERGUARD
#define DEF_CONTAINERGUARD

#include <string>
#include <map>
#include <iostream>

#include "../../debug.hpp"

template <typename ContainerType>
class Container
{
private:
    std::map<std::string, ContainerType> content;

public:
    Container();
    void add(const std::string&, ContainerType&);
    ContainerType& get(const std::string&);
};

template <typename ContainerType>
Container<ContainerType>::Container()
{

}

template <typename ContainerType>
void Container<ContainerType>::add(const std::string& name, ContainerType& value)
{
    this->content.emplace(name, value);
}

template <typename ContainerType>
ContainerType& Container<ContainerType>::get(const std::string& name)
{
    auto it = this->content.find(name);

    if (it == this->content.end())
        DebugLog(SH_ERR, "Can not find object with name " << name);
    else
        return it->second;
}

#endif // DEF_CONTAINERGUARD
