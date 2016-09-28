#ifndef DEF_SORT
#define DEF_SORT

#include "../../abstract/type.hpp"

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
};

#endif // DEF_SORT
