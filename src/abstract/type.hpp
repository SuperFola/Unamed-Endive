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
    POISON,
    ELECTRIC
};

enum SortilegeType {
    UniqueTargetAdvDamage,
    UniqueTargetAdvPoison,
    UniqueTargetAdvBurn,
    UniqueTargetAdvParalize,
    UniqueTargetAdvNerfCooldown,

    UniqueTargetUsHeal,
    UniqueTargetUsBuffCooldown,

    MultipleAdvDamage,
    MultipleAdvPoison,
    MultipleAdvBurn,
    MultipleAdvParalize,
    MultipleAdvNerfCooldown,

    MultipleUsHeal,
    MultipleUsBuffCooldown,

    Count
};

enum ActionType {
    Equipment,
    OnCrea
};

struct DexInfo {
    bool viewed;
    bool captured;
    Type type;
    int stade;
    std::string evolution;
    std::string file;
};

#endif // DEF_TYPE
