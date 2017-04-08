#include <iostream>
#include "../../../debug.hpp"

#include "pocket.hpp"

Pocket::Pocket(const std::string& name_, bool default_one) :
    name(name_)
    , default_pocket(default_one)
{
    this->default_udd.target_view = UNREACHABLE_VIEW_ID;
    this->default_udd.type = -1;
    this->default_udd.value = 0;
    this->default_udd.action = ObjAction::nothing;
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
                this->objects.push_back(object);
                DebugLog(SH_INFO, root["objects"][i]["id"].asInt() << " " << root["objects"][i]["quantity"].asInt());
            }
        }
    }
    else
    {
        Object* object = new Object(0, 1);
        this->objects.push_back(object);
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

ObjUDD Pocket::useObject(int id)
{
    if (0 <= id && id < this->objects.size())
    {
        if (this->objects[id]->getQuantity() > 0)
        {
            // minus one to quantity regarding to limited_use attrib
            if (ObjectsTable::isLimitedInUse(this->objects[id]))
                this->objects[id]->drop(1);

            ObjUDD ret;

            switch (ObjectsTable::getType(this->objects[id]))
            {
            case ObjType::player:
                ret.target_view = DEFAULT_VIEW_ID;
                ret.action = ObjAction::nothing;
                break;

            case ObjType::healpv:
            case ObjType::healpp:
            case ObjType::healstatus:
            case ObjType::levelup:
            case ObjType::lowercooldown:
                ret.target_view = MYCREATURES_VIEW_ID;
                ret.action = ObjAction::selectone;
                break;

            case ObjType::capture:
                ret.target_view = FIGHT_VIEW_ID;
                ret.action = ObjAction::selectone;
                break;

            default:
                // catching ObjType::none
                ret.target_view = UNREACHABLE_VIEW_ID;
                ret.action = ObjAction::nothing;
                break;
            }

            ret.type = ObjectsTable::getType(this->objects[id]);
            ret.value = ObjectsTable::getValue(this->objects[id]);

            DebugLog(SH_INFO, "type " << ret.type << " value " << ret.value << " action " << ret.action << " target view " << ret.target_view);

            return ret;
        }
        // else, quantity == 0
        return this->default_udd;
    }
    else
    {
        DebugLog(SH_ERR, "Can not find the object with the id " << id);
        return this->default_udd;
    }
}

void Pocket::drop_object(int id)
{
    // the id MUST be in range AND the object MUST be throwable
    if (0 <= id && id < this->objects.size() && ObjectsTable::isThrowable(this->objects[id]))
    {
        this->objects[id]->drop(1);
    }
    else
        DebugLog(SH_ERR, "Can not find the object with the id " << id << " or not throwable");
}

void Pocket::dropall_object(int id)
{
    // the id MUST be in range AND the object MUST be throwable
    if (0 <= id && id < this->objects.size() && ObjectsTable::isThrowable(this->objects[id]))
        pop<Object*>(&(this->objects), id);
    else
        DebugLog(SH_ERR, "Can not find the object with the id " << id << " or not throwable");
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
