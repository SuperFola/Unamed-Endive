#include "point.hpp"

// public
Point::Point() : x(0), y(0)
{

}

Point::Point(int nx, int ny) : x(nx), y(ny)
{

}

void Point::move(int mx, int my)
{
    this->x += mx;
    this->y += my;
}

void Point::move_x(int mx)
{
    this->x += mx;
}

void Point::move_y(int my)
{
    this->y += my;
}

void Point::set(int nx, int ny)
{
    this->x = nx;
    this->y = ny;
}

int Point::getX()
{
    return this->x;
}

int Point::getY()
{
    return this->y;
}
