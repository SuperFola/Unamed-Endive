#ifndef DEF_TYPES_TABLE
#define DEF_TYPES_TABLE

#include <vector>

#include "type.hpp"
#include "../json/json.h"

class TypesTable
{
private:
    TypesTable& operator=(const TypesTable&) {return self;}
    TypesTable(const TypesTable&) {}
    Json::Value root;
    std::vector<std::vector<float>> content;
    static class TypesTable self;
    TypesTable();

public:
    static bool load();
    static float atktype_on_deftype(Type, Type);
};

#endif // DEF_TYPES_TABLE
