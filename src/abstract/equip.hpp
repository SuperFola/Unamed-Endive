#ifndef DEF_EQUIP
#define DEF_EQUIP

#include <vector>
#include <string>
#include <random>

#include "../entities/creature.hpp"
#include "../json/json.h"
#include "functions.hpp"
#include "../entities/states.hpp"

class Equip {
private:
    // variables
    int max_crea;
    std::vector<Creature*> equip;
    Json::Value root;
    bool pc;

public:
    // methods
    Equip();
    bool load(const std::string&, bool pc=false);
    void save(const std::string&);
    bool add_creature(Creature*);
    bool remove_creature(int);
    Creature* getCrea(int);
    void increase_size();
    void increase_size(int);
    int getSize();
};

#endif // DEF_EQUIP
