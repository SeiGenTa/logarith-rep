#ifndef RANDOM_H
#define RANDOM_H
#include "Point.h"
#include <vector>
using namespace std;

float distanceBetweenTwoPoints(const Point* a,const Point *b);

pair<Point, Point> closestPairRandom(vector<Point> points);

#endif