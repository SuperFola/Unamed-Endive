#ifndef DEF_EQUIP
#define DEF_EQUIP

#include <vector>

#include "../entities/creature.hpp"

class Equip {
private:
    // methods
    // variables
    int max_crea;
    std::vector<Creature> equip;

public:
    // methods
    Equip();
    bool add_creature(Creature&);
    bool remove_creature(int);
    Creature& getCrea(int);
    void increase_size();
    int getSize();
};

#endif // DEF_EQUIP
