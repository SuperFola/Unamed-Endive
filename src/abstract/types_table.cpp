#include <iostream>

#include "types_table.hpp"

TypesTable::TypesTable()
{

}

bool TypesTable::load()
{
    std::ifstream file("assets/config/typestable.json");
    file >> this->root;

    for (int i=0; i < this->root.size(); i++)
    {
        std::vector<float> c_content;

        for (int j=0; j < this->root[i].size(); j++)
        {
            c_content.push_back(this->root[i][j].asFloat());
        }

        this->content.push_back(c_content);
    }

    return true;
}

float TypesTable::atktype_on_deftype(Type atk, Type def)
{
    return this->content[static_cast<int>(atk)][static_cast<int>(def)];
}
