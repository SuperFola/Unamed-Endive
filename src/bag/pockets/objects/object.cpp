#include <iostream>
#include "object.hpp"
#include "../../../abstract/functions.hpp"

Object::Object() :
    id(0)
    , quantity(0)
{

}

Json::Value Object::serialize()
{
    Json::Value value;
    value["id"] = this->id;
    value["quantity"] = this->quantity;
    value["action_type"] = this->act_type;

    return value;
}
