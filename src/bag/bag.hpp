#ifndef DEF_BAG
#define DEF_BAG

#include <vector>

#include "../json/json.h"
#include "pockets/pocket.hpp"

class Bag {
private:
    // variables
    std::vector<Pocket> pockets;

public:
    Bag();
    void add_pocket(Pocket);
    Pocket* getPocket(int);
    void serialize(Json::Value&);
};

#endif // DEF_BAG
