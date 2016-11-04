#ifndef DEF_BAG_OBJECT
#define DEF_BAG_OBJECT

#include <string>

#include "../../../json/json.h"

class Object {
private:
    int id;
    int quantity;

public:
    Object(int, int);
    Json::Value serialize();
    int getId();
    int getQuantity();
    void drop(int quantity=1);
};

#endif // DEF_BAG_OBJECT
