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
