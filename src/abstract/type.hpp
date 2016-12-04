#ifndef DEF_TYPE
#define DEF_TYPE

#include <string>

enum Type {
    NORMAL,
    FIRE,
    WATER,
    GRASS,
    FLYING,
    FIGHTING,
    POISON
};

enum SortilegeType {
    UniqueTargetAdv,
    UniqueTargetUs,
    MultipleAdv,
    MultipleUs
};

enum ActionType {
    Equipment = 0,
    OnCrea = 1
};

struct DexInfo {
    bool viewed;
    bool captured;
    Type type;
    int stade;
    std::string evolution;
};

#endif // DEF_TYPE
