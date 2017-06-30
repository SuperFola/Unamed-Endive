#include <iostream>
#include <fstream>

#include "types_table.hpp"

TypesTable TypesTable::self = TypesTable();

TypesTable::TypesTable()
{

}

bool TypesTable::load()
{
    std::ifstream file("assets/config/typestable.json");
    file >> self.root;

    for (int i=0; i < self.root.size(); i++)
    {
        std::vector<float> c_content;

        for (int j=0; j < self.root[i].size(); j++)
        {
            c_content.push_back(self.root[i][j].asFloat());
        }

        self.content.push_back(c_content);
    }

    return true;
}

float TypesTable::atktype_on_deftype(Type atk, Type def)
{
    return self.content[static_cast<int>(atk)][static_cast<int>(def)];
}
