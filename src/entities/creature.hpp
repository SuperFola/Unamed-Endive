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
    int id;
    Type type;
    int atk;
    int def;
    int life;
    int max_life;
    std::string name;
    Sort sortilege;
    State state;
    int level;
    std::vector<Competence> tree;

public:
    // methods
    Creature();
    bool load(int, Type&, int, int, int, int, std::string, State&, int, SortilegeType&, int, int, int);
    int getLife();
    int getLevel();
    std::string& getName();
    int getId();
    State getState();
    Type getType();
    int getAtk();
    int getDef();
    void update(std::vector<Creature>&);
    void print();
    Json::Value serialize();
};

#endif // DEF_CREATURE
