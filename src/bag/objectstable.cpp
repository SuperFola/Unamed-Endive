#include <iostream>
#include <fstream>
#include "../../debug.hpp"

#include "objectstable.hpp"

ObjectsTable ObjectsTable::instance = ObjectsTable();

ObjectsTable::ObjectsTable() :
    loaded(false)
{

}

bool ObjectsTable::load()
{
    DebugLog(SH_INFO, "Loading ObjectsTable");
    if (!instance.loaded)
    {
        instance.loaded = true;

        if (is_file_existing("assets/inventory/objects.json"))
            DebugLog(SH_OK, "Objects.json found");
        else
        {
            DebugLog(SH_ERR, "Can not find objects.json");
            return false;
        }

        std::ifstream file("assets/inventory/objects.json");
        file >> instance.root;

        for (int i=0; i < instance.root["objects"].size(); i++)
        {
            struct ObjData obj;
            obj.id = instance.root["objects"][i]["id"].asInt();
            obj.name = instance.root["objects"][i]["name"].asString();
            obj.desc = instance.root["objects"][i].get("desc", "").asString();
            obj.price = instance.root["objects"][i]["price"].asInt();
            obj.limited_use = instance.root["objects"][i]["limited_use"].asBool();
            obj.throwaway = instance.root["objects"][i]["throwaway"].asBool();
            obj.value = instance.root["objects"][i]["value"].asInt();
            obj.type = static_cast<ObjType>(instance.root["objects"][i]["type"].asInt() % 8);

            instance.objects_name.push_back(obj);

            DebugLog(SH_INFO, "id: " << obj.id << " name: " << obj.name << " desc: " << obj.desc << " price: " << obj.price <<
                     " use: " << obj.limited_use << " thow: " << obj.throwaway << " value: " << obj.value << " type: " << obj.type <<
                     " objects name size : " << instance.objects_name.size());
        }

        return true;
    }
    DebugLog(SH_WARN, "ObjectsTable was already loaded");
    return true;
}

std::string ObjectsTable::getName(Object* object)
{
    if (instance.loaded)
        return instance.objects_name[object->getId()].name;
    return "empty";
}

std::string ObjectsTable::getDesc(Object* object)
{
    if (instance.loaded)
        return instance.objects_name[object->getId()].desc;
    return "";
}

int ObjectsTable::getPrice(Object* object)
{
    if (instance.loaded)
        return instance.objects_name[object->getId()].price;
    return -1;
}

bool ObjectsTable::isLimitedInUse(Object* object)
{
    if (instance.loaded)
        return instance.objects_name[object->getId()].limited_use;
    return false;
}

bool ObjectsTable::isThrowable(Object* object)
{
    if (instance.loaded)
        return instance.objects_name[object->getId()].throwaway;
    return true;
}

int ObjectsTable::getValue(Object* object)
{
    if (instance.loaded)
        return instance.objects_name[object->getId()].value;
    return 0;
}

ObjType ObjectsTable::getType(Object* object)
{
    if (instance.loaded)
        return instance.objects_name[object->getId()].type;
    return ObjType::none;
}


























