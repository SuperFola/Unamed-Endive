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

enum ObjAction {
    nothing,
    selectone
};

// ObjectUseDataDescription
struct ObjUDD {
    int target_view;
    int type;
    int value;
    ObjAction action;
};

#endif // DEF_OBJTYPES




