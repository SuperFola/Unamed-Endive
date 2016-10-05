#ifndef DEF_BAG
#define DEF_BAG

#include <vector>

#include "pockets/pocket.hpp"

class Bag {
private:
    // variables
    std::vector<Pocket> pockets;

public:
    Bag();
    void add_pocket(Pocket);
    Pocket* getPocket(int);
};

#endif // DEF_BAG
