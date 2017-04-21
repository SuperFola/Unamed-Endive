#include <iostream>
#include "../../debug.hpp"

#include "messenger.hpp"

OMessenger OMessenger::instance = OMessenger();

OMessenger::OMessenger()
{

}

ObjUDD OMessenger::get()
{
    return instance.current;
}

void OMessenger::set(ObjUDD _new)
{
    instance.current = _new;
}

void OMessenger::empty()
{
    instance.current.target_view = UNREACHABLE_VIEW_ID;
    instance.current.value = 0;
    instance.current.type = -1;
    instance.current.action = ObjAction::nothing;
}

void OMessenger::lock(int lo)
{
    instance.lock = lo;
}

void OMessenger::unlock()
{
    instance.lock = 0;
}

int OMessenger::getLock()
{
    return instance.lock;
}
