#include <iostream>
#include "object.hpp"

Object::Object() :
    id(0)
    , quantity(0)
{

}

Json::Value Object::serialize()
{
    Json::value value;
    std::string content = "{\"id\": " + std::string(this->id) + ", \"quantity\": " + std::string(this->quantity) + ", \"action_type\": " + std::string(this->act_type) +  "}";
    content >> value;

    return value;
}
