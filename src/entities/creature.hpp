#ifndef DEF_CREATURE
#define DEF_CREATURE

#define DEFAULT_LIFE 100

#include <string>
#include <vector>

#include "../abstract/type.hpp"
#include "../abstract/competence.hpp"
#include "effects/sort.hpp"
#include "states.hpp"
#include "../json/json.h"

class Creature {
private:
    // variables
    Type type;
    int life;
    int max_life;
    std::string name;
    Sort sortilege;
    State state;
    std::vector<Competence> tree;
    int level;

public:
    // methods
    Creature();
    int getLife();
    int getLevel();
    void update(std::vector<Creature>&);
    Json::Value serialize();
};

#endif // DEF_CREATURE
