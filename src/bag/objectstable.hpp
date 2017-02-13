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
    std::map<int, struct ObjData> objects_name;
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
    static int getPrice(Object*);
    static bool isLimitedInUse(Object*);
    static bool isThrowable(Object*);
    static int getValue(Object*);
    static ObjType getType(Object*);
};

#endif // DEF_OBJECTSTABLE
