#include "view.hpp"

// public
View::View(int new_id) :
    id(new_id)
{

}

int View::getId()
{
    return this->id;
}

