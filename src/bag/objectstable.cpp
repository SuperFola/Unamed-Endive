#include <iostream>
#include <fstream>

#include "objectstable.hpp"

ObjectsTable ObjectsTable::instance = ObjectsTable();

ObjectsTable::ObjectsTable() :
    loaded(false)
{

}

bool ObjectsTable::load()
{
    if (!instance.loaded)
    {
        instance.loaded = true;
        std::ifstream file("assets/inventory/objects.json");
        file >> instance.root;

        for (int i=0; i < instance.root["objects"].size(); i++)
        {
            instance.objects_name[instance.root["object"][i]["id"].asInt()] = instance.root["objects"][i]["name"].asString();
        }

        return true;
    }
    std::cout << "ObjectsTable was already loaded" << std::endl;
    return true;
}

std::string ObjectsTable::getName(Object* object)
{
    if (instance.loaded)
        return instance.objects_name[object->getId()];
    return "empty";
}
