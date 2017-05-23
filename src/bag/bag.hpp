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
    std::vector<std::string> _pockets;
    std::string path;
    Json::Value root;

public:
    Bag();
    ~Bag();
    bool load(const std::string&);
    void add_pocket(Pocket*);
    Pocket* getPocket(int);
    void save(const std::string&);
};

#endif // DEF_BAG
