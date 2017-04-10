#ifndef DEF_CREATURE
#define DEF_CREATURE

#define DEFAULT_LIFE 100
#define DEFAULT_PP 30

#include <string>
#include <vector>
#include <math.h>

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
    int pp;
    int max_pp;
    int exp;
    std::string name;
    Sort sortilege;
    State state;
    int level;
    std::vector<Competence> tree;

public:
    // methods
    Creature();
    bool load(int, Type&, int, int, int, int, int, int, std::string, State&, int, int, SortilegeType&, int, int, int);
    bool loadfrom(Creature*);
    int getLife();
    int getMaxLife();
    int getLevel();
    int getPP();
    int getMaxPP();
    int getExp();
    std::string& getName();
    int getId();
    State getState();
    Type getType();
    int getAtk();
    int getDef();
    void update(std::vector<Creature>&);
    void print();
    Json::Value serialize();
    int gainExp(Creature*);

    static int calculateExpFromLevel(int);
    static int calculateLevelFromXp(int);

    bool healPV(int);
    bool healPP(int);
    bool healStatus(int);
    bool levelUP(int);
    bool lowercooldown(int);
};

#endif // DEF_CREATURE
