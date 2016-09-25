#ifndef DEF_BLOCK
#define DEF_BLOCK

class Block {
private:
    // variables
    bool solid;
    int id;

public:
    // methods
    Block();
    Block(int);
    Block(int, bool);
    bool is_solid();
    int getId();
};

#endif // DEF_BLOCK
