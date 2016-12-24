#ifndef DEF_TYPES_TABLE
#define DEF_TYPES_TABLE

#include <vector>

#include "type.hpp"
#include "../json/json.h"

class TypesTable
{
private:
    Json::Value root;
    std::vector<std::vector<float>> content;

public:
    TypesTable();
    bool load();
    float atktype_on_deftype(Type, Type);
};

#endif // DEF_TYPES_TABLE
