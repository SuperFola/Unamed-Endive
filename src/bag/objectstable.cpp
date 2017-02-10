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
    std::cout << "Loading ObjectsTable" << std::endl;
    if (!instance.loaded)
    {
        instance.loaded = true;

        if (is_file_existing("assets/inventory/objects.json"))
            std::cout << "Objects.json found" << std::endl;
        else
        {
            std::cout << "Can not find objects.json" << std::endl;
            return false;
        }

        std::ifstream file("assets/inventory/objects.json");
        file >> instance.root;

        for (int i=0; i < instance.root["objects"].size(); i++)
        {
            struct ObjType* obj;
            obj->id = instance.root["object"][i]["id"].asInt();
            obj->name = instance.root["objects"][i]["name"].asString();
            obj->desc = instance.root["objects"][i].get("desc", "");
            instance.objects_name[instance.root["object"][i]["id"].asInt()] = obj;
        }

        return true;
    }
    std::cout << "ObjectsTable was already loaded" << std::endl;
    return true;
}

std::string ObjectsTable::getName(Object* object)
{
    if (instance.loaded)
        return instance.objects_name[object->getId()]->name;
    return "empty";
}

std::string ObjectsTable::getDesc(Object* object)
{
    if (instance.loaded)
        return instance.objects_name[object->getId()]->desc;
    return "";
}
