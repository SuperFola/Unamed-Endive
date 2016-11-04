#include <iostream>
#include "pocket.hpp"
#include "../../abstract/functions.hpp"

Pocket::Pocket(const std::string& name_, bool default_one) :
    name(name_)
    , default_pocket(default_one)
{

}

bool Pocket::load(Json::Value& root)
{
    if (!this->default_pocket)
    {
        for (int i=0; i < root.size(); i++)
        {
            Object* object = new Object(root[i]["id"].asInt(), root[i]["quantity"].asInt());
            this->add_object(object);
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
        std::cout << "Can not find the object with the id " << id << std::endl;
        return this->objects[0]; // return first one to avoid problems
    }
}

void Pocket::drop_object(int id)
{
    if (0 <= id && id < this->objects.size())
        pop<Object*>(&(this->objects), id);
    else
        std::cout << "Can not find the object with the id " << id << std::endl;
}

Json::Value Pocket::serialize()
{
    Json::Value value;
    value["name"] = this->name;
    value["objects"] = Json::Value();

    for (int i=0; i < this->objects.size(); i++)
    {
        value["objects"].append(this->objects[i]->serialize());
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
