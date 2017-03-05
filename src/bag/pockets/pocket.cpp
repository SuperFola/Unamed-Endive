#include <iostream>
#include "../../../debug.hpp"

#include "pocket.hpp"

Pocket::Pocket(const std::string& name_, bool default_one) :
    name(name_)
    , default_pocket(default_one)
{

}

bool Pocket::load(Json::Value& root)
{
    if (!this->default_pocket)
    {
        this->name = root["name"].asString();

        for (int i=0; i < root["objects"].size(); i++)
        {
            if (!root["objects"][i].get("empty", false).asBool())
            {
                Object* object = new Object(root["objects"][i]["id"].asInt(), root["objects"][i]["quantity"].asInt());
                this->add_object(object);
            }
        }
    }
    else
    {
        Object* object = new Object(0, 1);
        this->add_object(object);
    }
    return true;
}

void Pocket::add_object(Object* object)
{
    this->objects.push_back(object);
}

Object* Pocket::getObject(int id)
{
    if (0 <= id && id < this->objects.size())
        return this->objects[id];
    else
    {
        DebugLog(SH_ERR, "Can not find the object with the id " << id);
        return this->objects[0]; // return first one to avoid problems
    }
}

void Pocket::useObject(int id)
{
    if (0 <= id && id < this->objects.size())
    {
        if (this->objects[id]->getQuantity() > 0)
        {
            this->objects[id];
        }
    }
    else
    {
        DebugLog(SH_ERR, "Can not find the object with the id " << id);
    }
}

void Pocket::drop_object(int id)
{
    if (0 <= id && id < this->objects.size())
    {
        this->objects[id]->drop(1);
    }
    else
        DebugLog(SH_ERR, "Can not find the object with the id " << id);
}

void Pocket::dropall_object(int id)
{
    if (0 <= id && id < this->objects.size())
        pop<Object*>(&(this->objects), id);
    else
        DebugLog(SH_ERR, "Can not find the object with the id " << id);
}

Json::Value Pocket::serialize()
{
    Json::Value value;
    value["name"] = this->name;
    value["objects"] = Json::Value();

    int i;
    for (i=0; i < this->objects.size(); i++)
    {
        value["objects"].append(this->objects[i]->serialize());
    }
    if (!i)
    {
        Json::Value content;
        content["empty"] = true;
        value["objects"].append(content);
    }

    return value;
}

std::string& Pocket::getName()
{
    return this->name;
}

int Pocket::getSize()
{
    return this->objects.size();
}
