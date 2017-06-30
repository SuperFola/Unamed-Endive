#pragma once

#include <string>
#include <vector>
#include <string>

#include "../../abstract/type.hpp"
#include "../../json/json.h"

class Creature;

class Sort {
private:
    // variables
    SortilegeType type;
    int damages;
    int targets;
    /// only if we add a competences' tree :
    std::string name;

public:
    // methods
    Sort();
    bool load(SortilegeType, int, int/*, const std::string&*/);
    bool loadfrom(Sort&);
    int act(Creature*);
    SortilegeType getType();
    int getTargets();
    Json::Value serialize();
};
