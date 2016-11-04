#include <iostream>
#include "object.hpp"
#include "../../../abstract/functions.hpp"

Object::Object(int id, int quantity)
{
    this->id = id;
    this->quantity = quantity;
}

Json::Value Object::serialize()
{
    Json::Value value;
    value["id"] = this->id;
    value["quantity"] = this->quantity;

    return value;
}

int Object::getId()
{
    return this->id;
}

int Object::getQuantity()
{
    return this->quantity;
}

void Object::drop(int quantity)
{
    this->quantity -= quantity;
    if (this->quantity < 0)
        this->quantity = 0;
}
