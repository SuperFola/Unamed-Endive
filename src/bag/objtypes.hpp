#ifndef DEF_OBJTYPES
#define DEF_OBJTYPES

#include <string>

enum ObjType {
    player,
    healpv,
    healpp,
    healstatus,
    capture,
    levelup,
    lowercooldown,
    none
};

struct ObjData {
    int id;
    std::string name;
    std::string desc;
    int price;
    bool limited_use;
    bool throwaway;
    int value;
    ObjType type;
};

#endif // DEF_OBJTYPES
