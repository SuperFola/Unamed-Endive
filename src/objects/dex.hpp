#ifndef DEF_DEX
#define DEF_DEX

#include <vector>
#include <map>
#include <string>
#include "../json/json.h"
#include "../abstract/type.hpp"
#include "../abstract/functions.hpp"

class Dex
{
private:
    Json::Value root;
    Json::Value root_player;
    std::map<std::string, struct DexInfo*> content;

public:
    Dex();
    bool load(const std::string&);
};

#endif // DEF_DEX
