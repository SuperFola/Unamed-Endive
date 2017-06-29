#ifndef DEF_CREATURE
#define DEF_CREATURE

#include <string>
#include <vector>
#include <math.h>

#include "../abstract/type.hpp"
#include "../abstract/competence.hpp"
#include "effects/sort.hpp"
#include "states.hpp"
#include "../json/json.h"

#define LIFEBAR_WIDTH 124.0f
#define LIFEBAR_HEIGHT 19.0f

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
    long int exp;
    std::string name;
    Sort sortilege;
    State state;
    int level;
    std::vector<Competence> tree;

public:
    // methods
    Creature();
    bool load(int, Type&, int, int, int, int, int, int, std::string, State&, int, long int, SortilegeType&, int, int);
    bool loadfrom(Creature*);
    int getLife();
    int getMaxLife();
    int getLevel();
    int getPP();
    int getMaxPP();
    long int getExp();
    std::string& getName();
    int getId();
    State getState();
    Type getType();
    int getAtk();
    int getDef();
    void update(std::vector<Creature*>&);
    void print();
    Json::Value serialize();
    int gainExp(Creature*);

    static long int calculateExpFromLevel(int);
    static int calculateLevelFromXp(long int);
    static int calculatePPFromLevel(int);
    static int calculateStatFromLevel(int);

    bool healPV(int);
    bool healPP(int);
    bool healStatus(int);
    bool levelUP(int);
};

#endif // DEF_CREATURE
