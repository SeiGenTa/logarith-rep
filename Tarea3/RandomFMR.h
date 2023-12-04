#ifndef RANDOFMR_H
#define RANDOFMR_H
#include "Point.h"
#include <vector>
using namespace std;

pair<Point, Point> closestPairRandomFMR(vector<Point> points, vector<vector<vector<Point *>>> &matriz);

#endif