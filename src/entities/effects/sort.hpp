#ifndef DEF_SORT
#define DEF_SORT

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
    bool load(SortilegeType, int, int);
    bool loadfrom(Sort&);
    void update();
    void act(std::vector<Creature*>&);
    Json::Value serialize();
};

#endif // DEF_SORT
