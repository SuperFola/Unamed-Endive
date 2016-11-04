#ifndef DEF_BAG
#define DEF_BAG

#include <vector>
#include <string>

#include "../json/json.h"
#include "pockets/pocket.hpp"
#include "../abstract/functions.hpp"

class Bag {
private:
    // variables
    std::vector<Pocket*> pockets;
    std::string path;
    Json::Value root;

public:
    Bag();
    bool load(const std::string&);
    void add_pocket(Pocket*);
    Pocket* getPocket(int);
    void serialize(Json::Value&);
};

#endif // DEF_BAG
