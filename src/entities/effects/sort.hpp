#ifndef DEF_SORT
#define DEF_SORT

#include <string>
#include <vector>

#include "../../abstract/type.hpp"

class Creature;

class Sort {
private:
    // methods
    // variables
    SortilegeType type;
    int damages;
    int cooldown;
    int current_cooldown;
    int targets;

public:
    // methods
    Sort();
    bool load(const std::string&);
    int getCooldown();
    void update();
    void act(std::vector<Creature>&);
};

#endif // DEF_SORT
