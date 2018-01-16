#include "point.hpp"

// public
Point::Point() : x(0.0f), y(0.0f)
{

}

Point::Point(float nx, float ny) : x(nx), y(ny)
{

}

void Point::move(float mx, float my)
{
    this->x += mx;
    this->y += my;
}

void Point::move_x(float mx)
{
    this->x += mx;
}

void Point::move_y(float my)
{
    this->y += my;
}

void Point::set(float nx, float ny)
{
    this->x = nx;
    this->y = ny;
}

float Point::getX()
{
    return this->x;
}

float Point::getY()
{
    return this->y;
}

sf::Vector2f Point::get()
{
    return sf::Vector2f(this->x, this->y);
}

int Point::squareDistanceTo(Point& _pos)
{
    int r = 0;

    r = (this->x - _pos.x) * (this->x - _pos.x) + (this->y - _pos.y) * (this->y - _pos.y);

    return r;
}

bool Point::intersect(Point& other, int spacingx, int spacingy)
{
    return (this->x <= other.x) && (other.x <= this->x + spacingx) && (this->y <= other.y) && (other.y <= this->y + spacingy);
}
