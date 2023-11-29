#ifndef DIVIDE_H
#define DIVIDE_H

#include "Point.h"
#include <vector>
using namespace std;

pair<Point, Point> closestPair(const vector<Point> &points);

float euclideanDistance(const Point &p1, const Point &p2);

#endif