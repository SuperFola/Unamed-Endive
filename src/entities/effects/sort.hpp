#ifndef DEF_SORT
#define DEF_SORT

#include <string>
#include <vector>

#include "../../abstract/type.hpp"
#include "../../json/json.h"

class Creature;

class Sort {
private:
    // variables
    SortilegeType type;
    int damages;
    int cooldown;
    int current_cooldown;
    int targets;

public:
    // methods
    Sort();
    bool load(SortilegeType, int, int, int);
    bool loadfrom(Sort&);
    int getCooldown();
    bool lowercooldown(int);
    void update();
    void act(std::vector<Creature*>&);
    Json::Value serialize();
};

#endif // DEF_SORT
