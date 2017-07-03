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
#include "../abstract/types_table.hpp"

#define LIFEBAR_WIDTH 124.0f
#define LIFEBAR_HEIGHT 19.0f
#define DEFAULT_LIFE 100
#define DEFAULT_PP 30

#define DEAD_BUR 1
#define DEAD_PSN 2
#define LOST_BUR 1
#define LOST_PAR 2
#define LOST_PSN 3

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
    bool load(int, Type&, int, int, int, int, int, int, std::string, State, int, long int, SortilegeType&, int, int);
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
    std::string getStringState();
    Type getType();
    int getAtk();
    int getDef();
    void print();
    Json::Value serialize();
    int gainExp(Creature*);
    void attack(Creature*);
    Sort* getSort();
    void setStatus(State);
    void atk_heal_pv(int);
    void atk_heal_status();
    int updateState();

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
