#ifndef DEF_BAG_OBJECT
#define DEF_BAG_OBJECT

#include <string>

#include "../../../json/json.h"
#include "../../../abstract/type.hpp"

class Object {
private:
    int id;
    int quantity;
    ActionType act_type;

public:
    Object();
    Json::Value serialize();
};

#endif // DEF_BAG_OBJECT
