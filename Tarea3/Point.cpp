
#include <bits/stdc++.h>

struct Point
{
    double x, y;

    Point(double x, double y) : x(x), y(y) {}

};

/*
calculate the distance between two points
*/
double distanceBetweenTwoPoints(const Point *a, const Point *b)
{
    return sqrt(pow(a->x - b->x, 2) + (pow(a->y - b->y, 2)));
}