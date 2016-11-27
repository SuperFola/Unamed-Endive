#ifndef DEF_PNJMANAGER
#define DEF_PNJMANAGER

#include <map>
#include <vector>
#include <string>

#include "pnj.hpp"

class PNJManager
{
private:
    std::map<int, std::vector<PNJ>> pnjs;

public:
    PNJManager();
    void add_pnj_on_map(int, PNJ, std::string);
    int countPNJonMap(int);
    PNJ& getPNJonMap(int, int);
};

#endif // DEF_PNJMANAGER
