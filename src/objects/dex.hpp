#ifndef DEF_DEX
#define DEF_DEX

#include <vector>
#include <map>
#include <string>
#include "../json/json.h"
#include "../abstract/type.hpp"

class Dex
{
private:
    Json::Value root;
    std::map<std::string, struct DexInfo*> content;

public:
    Dex();
    bool load();
};

#endif // DEF_DEX
