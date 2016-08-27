#ifndef DEF_POINT
#define DEF_POINT

class Point {
private:
    int x;
    int y;

public:
    Point();
    Point(int, int);
    void move(int, int);
    void move_x(int);
    void move_y(int);
    void set(int, int);
    int getX();
    int getY();
};

#endif // DEF_POINT
