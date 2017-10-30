#ifndef DEF_SCRIPTING_TYPES
#define DEF_SCRIPTING_TYPES

struct svar_t {
    int ivar;
    float fvar;
    char* cvar;
    int bvar;

    const char* kind;
};

struct fight_opponent {
    int id;
    int lvl;
    int life;
    int atk;
    int def;
    int sort_type;
    int sort_dmg;
    int sort_targets;
};

#endif // DEF_SCRIPTING_TYPES
