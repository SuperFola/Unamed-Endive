#ifndef DEF_OBJECTSTABLE
#define DEF_OBJECTSTABLE

#include <map>
#include <string>

#include "pockets/objects/object.hpp"
#include "../json/json.h"

class ObjectsTable
{
private:
    // variables
    std::map<int, std::string> objects_name;
    static class ObjectsTable instance;
    Json::Value root;
    bool loaded;
    // methods
    ObjectsTable();

public:
    static ObjectsTable& Instance();
    static bool load();
    static std::string getName(Object*);
};

#endif // DEF_OBJECTSTABLE
