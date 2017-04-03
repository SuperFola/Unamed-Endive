#ifndef DEF_OBJMESSENGER
#define DEF_OBJMESSENGER

#include "../bag/objtypes.hpp"

class OMessenger
{
private:
    static class OMessenger instance;
    OMessenger();
    ObjUDD current;

public:
    static ObjUDD get();
    static void set(ObjUDD);
    static void empty();
};

#endif // DEF_OBJMESSENGER
