#ifndef DEF_POCKET
#define DEF_POCKET

#include <vector>
#include <string>

#include "../../json/json.h"
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
    Json::Value serialize();
};

#endif // DEF_POCKET
