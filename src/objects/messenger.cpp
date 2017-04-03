#include <iostream>
#include "../../debug.hpp"

#include "messenger.hpp"

OMessenger OMessenger::instance = OMessenger();

ObjUDD OMessenger::get()
{
    return instance.current;
}

void OMessenger::set(ObjUDD _new)
{
    instance.curent = _new;
}

void OMessenger::empty()
{
    instance.current.target_view = UNREACHABLE_VIEW_ID;
    instance.current.value = 0;
    instance.current.type = -1;
    instance.current.action = ObjAction::nothing;
}
