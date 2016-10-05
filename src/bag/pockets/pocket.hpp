#ifndef DEF_POCKET
#define DEF_POCKET

#include <vector>
#include <string>

#include "objects/object.hpp"

class Pocket {
private:
    std::vector<Object> objects;
    std::string name;

public:
    Pocket(const std::string&);
    void add_object(Object);
    Object* getObject(int);
    void drop_object(int);
};

#endif // DEF_POCKET
