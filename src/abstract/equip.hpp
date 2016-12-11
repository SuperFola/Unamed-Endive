#ifndef DEF_EQUIP
#define DEF_EQUIP

#include <vector>
#include <string>

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

public:
    // methods
    Equip();
    bool load(const std::string&);
    void save(const std::string&);
    bool add_creature(Creature&);
    bool remove_creature(int);
    Creature* getCrea(int);
    void increase_size();
    int getSize();
};

#endif // DEF_EQUIP
