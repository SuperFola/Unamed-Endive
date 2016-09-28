#ifndef DEF_CREATURE
#define DEF_CREATURE

#define DEFAULT_LIFE 100

#include <string>
#include <vector>

#include "../abstract/type.hpp"
#include "../abstract/competence.hpp"
#include "sort.hpp"
#include "states.hpp"

class Creature {
private:
    // methods
    // variables
    Type type;
    int life;
    int max_life;
    std::string name;
    int id;
    bool revealed;
    Sort sortilege;
    State state;
    std::vector<Competence> tree;
    int level;

public:
    // methods
    Creature();
    Creature(int);
    Creature(int, bool);
    void reveal();
    bool is_revealed();
    int getLife();
    int getLevel();
};

#endif // DEF_CREATURE
