#ifndef DEF_POINT
#define DEF_POINT

#include <SFML/System/Vector2.hpp>

class Point {
private:
    float x;
    float y;

public:
    Point();
    Point(float, float);
    void move(float, float);
    void move_x(float);
    void move_y(float);
    void set(float, float);
    float getX();
    float getY();
    sf::Vector2f get();
    int squareDistanceTo(Point&);
    bool intersect(Point& other, int spacingx=64, int spacingy=64);
};

#endif // DEF_POINT
