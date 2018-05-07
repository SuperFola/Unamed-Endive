#ifndef DEF_BLOCK
#define DEF_BLOCK

#include <string>
#include <map>

class Block {
private:
    // variables
    bool solid;
    int id;
    int priority;

public:
    // methods
    Block();
    Block(int, bool, int);
    bool is_solid();
    int getId();
    void setId(int);
    void setSolid(bool);
    int getPriority();
};

#endif // DEF_BLOCK
