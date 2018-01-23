#ifndef DEF_BLOCK
#define DEF_BLOCK

#include <string>
#include <map>

class Block {
private:
    // variables
    bool solid;
    int id;

public:
    // methods
    Block();
    Block(int, bool);
    bool is_solid();
    int getId();
    void setId(int);
    void setSolid(bool);
};

#endif // DEF_BLOCK
