#ifndef DEF_BLOCK
#define DEF_BLOCK

#include <string>
#include <map>

#include "triggers.hpp"

using dict_act = std::map<std::string, std::string>;  // temp

class Block {
private:
    // variables
    bool solid;
    int id;
    dict_act trigger;
    bool reset_trigger;
    Trigger trigg_kind;


public:
    // methods
    Block();
    Block(int, bool);
    Block(int, bool, bool);
    bool is_solid();
    int getId();
    void setTriggerReset(bool);
    void add_trigger(const std::string&, const std::string&, Trigger); // temp
};

#endif // DEF_BLOCK
