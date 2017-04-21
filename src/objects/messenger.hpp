#ifndef DEF_OBJMESSENGER
#define DEF_OBJMESSENGER

#include "../bag/objtypes.hpp"

class OMessenger
{
private:
    static class OMessenger instance;
    OMessenger();
    ObjUDD current;
    int lock;

public:
    static ObjUDD get();
    static void set(ObjUDD);
    static void empty();
    static void setlock(int);
    static void unlock();
    static int getLock();
};

#endif // DEF_OBJMESSENGER
