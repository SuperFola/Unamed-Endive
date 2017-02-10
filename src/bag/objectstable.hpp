#ifndef DEF_OBJECTSTABLE
#define DEF_OBJECTSTABLE

#include <map>
#include <string>

#include "pockets/objects/object.hpp"
#include "../json/json.h"
#include "../abstract/functions.hpp"
#include "objtypes.hpp"

class ObjectsTable
{
private:
    // variables
    std::map<int, struct ObjType> objects_name;
    static class ObjectsTable instance;
    Json::Value root;
    bool loaded;
    // methods
    ObjectsTable();

public:
    static ObjectsTable& Instance();
    static bool load();
    static std::string getName(Object*);
    static std::string getDesc(Object*);
};

#endif // DEF_OBJECTSTABLE
